#pragma once

#include <ActionGame.h>
#include <XEngine.h>
#include <Network.h>

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

private:
	void HandleNewClients();
	void ProcessClientMessage();
	void BroadcastCreate(GameObject* gameObject);
	void BroadcastWorldUpdate();

	struct Client
	{
		Network::TCPSocket* clientSocket;
		uint32_t networkId;
	};

	std::vector<Client> m_clients;
	Network::TCPSocket m_listener;
};

