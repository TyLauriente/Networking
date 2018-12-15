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
	if (m_update)
	{
		m_playerBoard.Update(deltaTime);
		for (auto& opponent : m_opponentBoards)
		{
			opponent.playerBoard.Update(deltaTime);
		}
		if (m_playerBoard.CanMoveShape())
		{
			if (X::IsKeyDown(X::Keys::LEFT))
			{
				if (m_playerBoard.SetBoardCommand(BoardCommand::MoveLeft))
				{
					SendBoardCommandToServer(BoardCommand::MoveLeft);
				}
			}
			else if (X::IsKeyDown(X::Keys::RIGHT))
			{
				if (m_playerBoard.SetBoardCommand(BoardCommand::MoveRight))
				{
					SendBoardCommandToServer(BoardCommand::MoveRight);
				}
			}
			else if (X::IsKeyDown(X::Keys::UP))
			{
				if (m_playerBoard.SetBoardCommand(BoardCommand::RotateLeft))
				{
					SendBoardCommandToServer(BoardCommand::RotateLeft);
				}
			}
			else if (X::IsKeyDown(X::Keys::DOWN))
			{
				if (m_playerBoard.SetBoardCommand(BoardCommand::MoveDown))
				{
					SendBoardCommandToServer(BoardCommand::MoveDown);
				}
			}
		}
		
		if (m_playerBoard.NeedsNetworkPush())
		{
			SendShapeToServer(m_playerBoard.GetShapeToSpawn());
			m_playerBoard.Push();
		}
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

void ClientManager::SendBoardCommandToServer(BoardCommand command)
{
	Network::MemoryStream memStream;
	Network::StreamWriter writer(memStream);
	writer.Write(NetworkCommand::BoardCommand);
	writer.Write(command);
	SendMessageToServer(memStream);
}

void ClientManager::SendShapeToServer(Shapes shape)
{
	Network::MemoryStream memStream;
	Network::StreamWriter writer(memStream);
	writer.Write(NetworkCommand::SpawnShape);
	writer.Write(shape);
	SendMessageToServer(memStream);
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

		while (reader.GetRemainingDataSize() > 0 && reader.GetRemainingDataSize() < (uint32_t)std::size(buffer))
		{
			uint32_t messageType;
			reader.Read(messageType);
			int networkId;
			X::Math::Vector2 newPos;
			switch (static_cast<NetworkCommand>(messageType))
			{
			case NetworkCommand::SetupGame:
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
				m_opponentBoards.back().playerBoard.SetCanSpawnShapes(false);
				break;
			case NetworkCommand::StartGame:
				m_update = true;
				break;
			case NetworkCommand::BoardCommand:
				reader.Read(networkId);
				BoardCommand command;
				reader.Read(command);
				if (networkId != m_clientId)
				{
					for (auto& opponent : m_opponentBoards)
					{
						if (opponent.networkId == networkId)
						{
							opponent.playerBoard.SetBoardCommand(command);
						}
					}
				}
				break;
			case NetworkCommand::SpawnShape:
				reader.Read(networkId);
				Shapes shape;
				reader.Read(shape);
				if (networkId != m_clientId)
				{
					for (auto& opponent : m_opponentBoards)
					{
						if (opponent.networkId == networkId)
						{
							opponent.playerBoard.SpawnShape(shape);
						}
					}
				}
				break;
			default:
				XASSERT(false, "What is this?");
				break;
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