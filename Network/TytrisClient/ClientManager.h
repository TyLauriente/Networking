#pragma once

#include <TytrisGame.h>
#include <map>

class ClientManager
{
public:
	static void StaticInitialize();
	static void StaticTerminate();
	static ClientManager* Get();

	void Initialize();
	void Terminate();

	void Update(float deltaTime);
	void Render();

	bool ConnectToServer(const char* host, uint16_t port);

	bool SendMessageToServer(Network::MemoryStream& memStream);
	bool HandleMessage();

	uint32_t GetClientId() const { return m_clientId; }

	void RenderOpponentBoards();

private:
	struct Opponent
	{
		TytrisBoard playerBoard;
		uint32_t networkId;
	};

	Network::TCPSocket m_socket;
	uint32_t m_clientId;

	TytrisBoard m_playerBoard;
	std::vector<Opponent> m_opponentBoards;
};

