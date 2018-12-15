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
	void SendBoardCommandToServer(BoardCommand command);
	void SendShapeToServer(Shapes shape);

	bool HandleMessage();

	uint32_t GetClientId() const { return m_clientId; }

	void RenderOpponentBoards();

	bool IsDead() const { return m_dead; }

	bool Won() const { return m_win; }

private:
	struct Opponent
	{
		TytrisBoard playerBoard;
		uint32_t networkId;
	};
	

	Network::TCPSocket m_socket;
	uint32_t m_clientId;

	TytrisBoard m_playerBoard;
	bool m_dead{ false };
	bool m_win{ false };
	std::vector<Opponent> m_opponentBoards;
	bool m_update{ false };
};

