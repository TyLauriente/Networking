#include "ClientNetworkManager.h"

namespace
{
	ClientNetworkManager* sInstance{ nullptr };
}

void ClientNetworkManager::StaticInitialize()
{
	XASSERT(sInstance == nullptr, "Manager already initialized.");
	sInstance = new ClientNetworkManager();
	sInstance->Initialize();
}

void ClientNetworkManager::StaticTerminate()
{
	XASSERT(sInstance != nullptr, "Manager already terminated.");
	sInstance->Terminate();
	X::SafeDelete(sInstance);
}

ClientNetworkManager* ClientNetworkManager::Get()
{
	XASSERT(sInstance != nullptr, "Manager not initialized.");
	return sInstance;
}


void ClientNetworkManager::Initialize()
{
	Network::Initialize();
}

void ClientNetworkManager::Terminate()
{
	Network::Terminate();
}


bool ClientNetworkManager::ConnectToServer(const char* host, uint16_t port)
{
	Network::SocketAddress address;
	return
		m_socket.Open() &&
		m_socket.SetNoDelay(true) &&
		m_socket.SetNonBlocking(true) &&
		m_socket.Connect({ host, port });
}


bool ClientNetworkManager::SendMessageToServer(Network::MemoryStream& memStream)
{
	int bytesSent = m_socket.Send(memStream.GetData(), memStream.GetHead());
	return bytesSent != SOCKET_ERROR;
}

bool ClientNetworkManager::HandleMessage()
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
			switch (MessageType(messageType))
			{
			case MessageType::Msg_Snapshot:
				GameObjectManager::Get()->Deserialize(reader);
				break;
			case MessageType::Assignment:
				reader.Read(m_clientId);
				break;
			case MessageType::Msg_Create:
				GameObjectManager::Get()->CreateGameObject(reader);
				break;
			case MessageType::Msg_Update:
				GameObjectManager::Get()->Update(reader);
				break;
			}
		}
	}
	return true;
}
