#pragma once

#include <TytrisGame.h>
#include <XEngine.h>
#include <Network.h>

constexpr float PLAYER_Y_POS = 360.0f;
constexpr float PLAYER_X_POS[3] = { 200.5f, 616.0f, 1025.0f };

class ServerManager
{
public:
	static void StaticInitialize();
	static void StaticTerminate();
	static ServerManager* Get();

	ServerManager();
	~ServerManager();

	void Initialize();
	void Terminate();

	void Run(uint16_t port);

	void BroadcastMessage(Network::MemoryStream& memStream);

private:
	void HandleNewClients();
	void ProcessClientMessage();

	struct Client
	{
		Network::TCPSocket* clientSocket;
		uint32_t networkId;
		TytrisBoard playerBoard;
	};

	std::vector<Client> m_clients;
	Network::TCPSocket m_listener;
};

