#include "ServerManager.h"

namespace
{
	ServerManager* sInstance = nullptr;
}

void ServerManager::StaticInitialize()
{
	XASSERT(sInstance == nullptr, "Manager already initialized.");
	sInstance = new ServerManager();
	sInstance->Initialize();
}

void ServerManager::StaticTerminate()
{
	XASSERT(sInstance != nullptr, "Manager already terminated.");
	sInstance->Terminate();
	delete sInstance;
	sInstance = nullptr;
}

ServerManager* ServerManager::Get()
{
	XASSERT(sInstance != nullptr, "Manager not initialized.");
	return sInstance;
}


ServerManager::ServerManager()
{
}


ServerManager::~ServerManager()
{

}

void ServerManager::Initialize()
{
	Network::Initialize();
}

void ServerManager::Terminate()
{
	Network::Terminate();
}

void ServerManager::Run(uint16_t port, int numberOfPlayers)
{
	m_numberOfPlayers = numberOfPlayers;
	// Create a listener socket using TCP/IP
	if (!m_listener.Open() || !m_listener.SetNoDelay(true) || !m_listener.SetNonBlocking(true))
	{
		return;
	}

	// Bind the address to our listener socket and listen for connection
	Network::SocketAddress serverInfo(port);
	if (!m_listener.Bind(serverInfo) || !m_listener.Listen())
	{
		return;
	}

	while (true)
	{
		// Accept any incoming client connection
		if (m_clients.size() < m_numberOfPlayers)
		{
			HandleNewClients();
		}
		else if (!m_gameStarted)
		{
			m_gameStarted = true;
			StartGame();
		}

		// Check client message
		ProcessClientMessage();


		// Sleep for 10ms
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	// Close listener socket
	m_listener.Close();
}


void ServerManager::BroadcastMessage(Network::MemoryStream& memStream)
{
	for (auto& client : m_clients)
	{
		client.clientSocket->Send(memStream.GetData(), memStream.GetHead());
	}
}

void ServerManager::StartGame()
{
	Network::MemoryStream stream;
	Network::StreamWriter writer(stream);
	writer.Write(NetworkCommand::StartGame);
	BroadcastMessage(stream);
}

void ServerManager::HandleNewClients()
{
	Network::SocketAddress address;
	Network::TCPSocket* clientSocket = m_listener.Accept(address);
	if (clientSocket == nullptr)
	{
		return;
	}

	const int clientId = (int)m_clients.size();


	printf("Client joined!\n");


	// Send SetupGame command with clients board pos
	Network::MemoryStream memStream;
	Network::StreamWriter writer(memStream);
	writer.Write(NetworkCommand::SetupGame);
	writer.Write(PLAYER_X_POS[m_clients.size()]);
	writer.Write(PLAYER_Y_POS);

	// Create a new player for this client

	writer.Write(NetworkCommand::AssignId);
	writer.Write(clientId);
	clientSocket->Send(memStream.GetData(), memStream.GetHead());

	printf("Assigned client to be network id %d\n", clientId);


	// Add new client
	Client newClient;
	newClient.clientSocket = clientSocket;
	newClient.networkId = clientId;
	m_clients.push_back(newClient);


	// Give New Client Current Player Boards
	if (m_clients.size() > 0)
	{
		int clientNumber{ 0 };
		for (auto& client : m_clients)
		{
			if (client.networkId != clientId)
			{
				memStream.Reset();
				writer.Write(NetworkCommand::AddPlayer);
				writer.Write(client.networkId);
				writer.Write(PLAYER_X_POS[clientNumber]);
				writer.Write(PLAYER_Y_POS);
				newClient.clientSocket->Send(memStream.GetData(), memStream.GetHead());
			}
			clientNumber++;
		}
	}

	// Give Previous Clients the new client
	memStream.Reset();
	writer.Write(NetworkCommand::AddPlayer);
	writer.Write(clientId);
	writer.Write(PLAYER_X_POS[m_clients.size() - 1]);
	writer.Write(PLAYER_Y_POS);
	for (auto& client : m_clients)
	{
		if (client.networkId != clientId)
		{
			client.clientSocket->Send(memStream.GetData(), memStream.GetHead());
		}
	}
}

void ServerManager::ProcessClientMessage()
{
	for (size_t i = 0; i < m_clients.size(); ++i)
	{
		Client& client = m_clients[i];
		if (client.clientSocket == nullptr)
		{
			continue;
		}
		uint8_t buffer[16384];
		int bytesReceived = client.clientSocket->Receive(buffer, (int)std::size(buffer));
		if (bytesReceived > 0)
		{
			Network::MemoryStream memStream(buffer, bytesReceived);
			Network::StreamReader reader(memStream);

			while (reader.GetRemainingDataSize() > 0 && reader.GetRemainingDataSize() < (uint32_t)std::size(buffer))
			{
				int commandType;
				reader.Read(commandType);
				Network::MemoryStream stream;
				Network::StreamWriter writer(stream);
				switch (static_cast<NetworkCommand>(commandType))
				{
				case NetworkCommand::BoardCommand:
					BoardCommand command;
					reader.Read(command);
					stream.Reset();
					writer.Write(NetworkCommand::BoardCommand);
					writer.Write(client.networkId);
					writer.Write(command);
					BroadcastMessage(stream);
					break;
				case NetworkCommand::SpawnShape:
					Shapes shape;
					reader.Read(shape);
					stream.Reset();
					writer.Write(NetworkCommand::SpawnShape);
					writer.Write(client.networkId);
					writer.Write(shape);
					BroadcastMessage(stream);
					break;
				}
			}
		}
		else if (bytesReceived == SOCKET_ERROR)
		{
			m_clients[i].clientSocket->Close();
			X::SafeDelete(m_clients[i].clientSocket);

			//TODO: Delete the character here
		}
	}
}
