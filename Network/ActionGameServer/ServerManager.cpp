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

void ServerManager::Run(uint16_t port)
{
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
		HandleNewClients();

		// Check client message
		ProcessClientMessage();

		// Update world for 10ms
		GameObjectManager::Get()->Update(0.01f);

		// Send update to all clients
		BroadcastWorldUpdate();

		// Sleep for 10ms
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	// Close listener socket
	m_listener.Close();
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


	// Send client a snapshot
	Network::MemoryStream memStream;
	Network::StreamWriter writer(memStream);
	writer.Write(MessageType::Msg_Snapshot);
	GameObjectManager::Get()->Serialize(writer, false);
	clientSocket->Send(memStream.GetData(), memStream.GetHead());

	// Create a new player for this client
	Character* newCharacter = (Character*)GameObjectManager::Get()->CreateGameObject(Character::ClassId);
	
	memStream.Reset();
	writer.Write(MessageType::Assignment);
	writer.Write(newCharacter->GetNetworkId());
	clientSocket->Send(memStream.GetData(), memStream.GetHead());

	printf("Assigned client to be network id %d\n", newCharacter->GetNetworkId());

	// Add new client
	Client newClient;
	newClient.clientSocket = clientSocket;
	newClient.networkId = newCharacter->GetNetworkId();
	m_clients.push_back(newClient);

	// Tell everyone abou the new character
	newCharacter->SetPosition({ X::RandomFloat(100.0f, 1180.0f), X::RandomFloat(100.0f, 620.0f) });
	BroadcastCreate(newCharacter);

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

			while (reader.GetRemainingDataSize() > 0)
			{
				uint32_t messageType;
				reader.Read(messageType);
				switch (MessageType(messageType))
				{
				case MessageType::Msg_Command:
					Command command;
					command.Deserialize(reader);
					Character* myCharacter = (Character*)GameObjectManager::Get()->
						FindGameObject(client.networkId);
					myCharacter->Apply(command);
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

void ServerManager::BroadcastCreate(GameObject* gameObject)
{
	Network::MemoryStream memStream;
	Network::StreamWriter writer(memStream);
	writer.Write(MessageType::Msg_Create);
	writer.Write(gameObject->GetClassId());
	writer.Write(gameObject->GetNetworkId());
	gameObject->Serialize(writer);

	// Send to all clients
	for (auto& client : m_clients)
	{
		if (client.clientSocket != nullptr)
		{
			client.clientSocket->Send(memStream.GetData(), memStream.GetHead());
		}
	}
}

void ServerManager::BroadcastWorldUpdate()
{
	Network::MemoryStream memStream;
	Network::StreamWriter writer(memStream);
	writer.Write(MessageType::Msg_Update);
	GameObjectManager::Get()->Serialize(writer, true);

	// Send to all clients
	if (memStream.GetHead() > 4)
	{
		for (auto& client : m_clients)
		{
			if (client.clientSocket != nullptr)
			{
				client.clientSocket->Send(memStream.GetData(), memStream.GetHead());
			}
		}
	}
}
