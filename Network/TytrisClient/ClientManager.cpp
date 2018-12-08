#include "ClientManager.h"

namespace
{
	ClientManager* sInstance{ nullptr };
}

void ClientManager::StaticInitialize()
{
	XASSERT(sInstance == nullptr, "Manager already initialized.");
	sInstance = new ClientManager();
	sInstance->Initialize();
}

void ClientManager::StaticTerminate()
{
	XASSERT(sInstance != nullptr, "Manager already terminated.");
	sInstance->Terminate();
	X::SafeDelete(sInstance);
}

ClientManager* ClientManager::Get()
{
	XASSERT(sInstance != nullptr, "Manager not initialized.");
	return sInstance;
}


void ClientManager::Initialize()
{
	Network::Initialize();
}

void ClientManager::Terminate()
{
	Network::Terminate();
}



void ClientManager::Update(float deltaTime)
{
	m_playerBoard.Update(deltaTime);
	if (m_playerBoard.NeedsNetworkPush())
	{
		Network::MemoryStream memStream;
		Network::StreamWriter writer(memStream);
		writer.Write(NetworkCommand::UpdateBoard);
		m_playerBoard.Serialize(writer);
		m_playerBoard.Push();
		ClientManager::Get()->SendMessageToServer(memStream);
	}
}

void ClientManager::Render()
{
	m_playerBoard.Render();
	RenderOpponentBoards();
}


bool ClientManager::ConnectToServer(const char* host, uint16_t port)
{
	Network::SocketAddress address;
	return
		m_socket.Open() &&
		m_socket.SetNoDelay(true) &&
		m_socket.SetNonBlocking(true) &&
		m_socket.Connect({ host, port });
}


bool ClientManager::SendMessageToServer(Network::MemoryStream& memStream)
{
	int bytesSent = m_socket.Send(memStream.GetData(), memStream.GetHead());
	return bytesSent != SOCKET_ERROR;
}

bool ClientManager::HandleMessage()
{
	uint8_t buffer[16384];
	int bytesReceived = m_socket.Receive(buffer, (int)std::size(buffer));
	if (bytesReceived == SOCKET_ERROR)
	{
		return false;
	}

	if (bytesReceived > 0)
	{
		Network::MemoryStream memStream(buffer, bytesReceived);
		Network::StreamReader reader(memStream);

		while (reader.GetRemainingDataSize() > 0)
		{
			uint32_t messageType;
			reader.Read(messageType);
			int networkId;
			X::Math::Vector2 newPos;
			switch (static_cast<NetworkCommand>(messageType))
			{
			case NetworkCommand::StartGame:
				reader.Read(newPos.x);
				reader.Read(newPos.y);
				m_playerBoard.SetPosition(newPos);
				m_playerBoard.XInitialize();
				break;
			case NetworkCommand::AssignId:
				reader.Read(m_clientId);
				break;
			case NetworkCommand::AddPlayer:
				reader.Read(networkId);
				reader.Read(newPos.x);
				reader.Read(newPos.y);
				m_opponentBoards.push_back({ TytrisBoard(), static_cast<uint32_t>(networkId) });
				m_opponentBoards.back().playerBoard.SetPosition(newPos);
				m_opponentBoards.back().playerBoard.XInitialize();
				break;
			case NetworkCommand::UpdateBoard:
				reader.Read(networkId);
				if (networkId == m_clientId)
				{
					m_playerBoard.Deserialzie(reader);
				}
				else
				{
					for (auto& opponent : m_opponentBoards)
					{
						if (opponent.networkId == networkId)
						{
							opponent.playerBoard.Deserialzie(reader);
						}
					}
				}
			}
		}
	}
	return true;
}

void ClientManager::RenderOpponentBoards()
{
	for (auto& opponent : m_opponentBoards)
	{
		opponent.playerBoard.Render();
	}
}