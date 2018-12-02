#pragma once

#include <ActionGame.h>

class ClientNetworkManager
{
public:
	static void StaticInitialize();
	static void StaticTerminate();
	static ClientNetworkManager* Get();

	void Initialize();
	void Terminate();

	bool ConnectToServer(const char* host, uint16_t port);

	bool SendMessageToServer(Network::MemoryStream& memStream);
	bool HandleMessage();

	uint32_t GetClientId() const { return m_clientId; }

private:
	Network::TCPSocket m_socket;
	uint32_t m_clientId;
};

