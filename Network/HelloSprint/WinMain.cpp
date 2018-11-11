#include <Network.h>
#include <XEngine.h>

struct Runner
{
	int frameId;
	X::Math::Vector2 position;
};
std::mutex mutex;
std::thread recvThread;
Network::TCPSocket listenSocket;
Network::TCPSocket *otherSocket = nullptr;
Network::SocketAddress clientAddress(8888);

X::TextureId textureIds[2];
std::vector<Runner> runners;
bool isHost = false;
bool gameStarted = false;

void GameInit()
{
	Network::Initialize();
	textureIds[0] = X::LoadTexture("mario_walk_01.png");
	textureIds[1] = X::LoadTexture("mario_walk_02.png");

	runners.push_back(Runner());
	runners.back().frameId = 0;
	runners.back().position = { 100.0f, 200.0f };
	runners.push_back(Runner());
	runners.back().frameId = 0;
	runners.back().position = { 100.0f, 400.0f };
}

void GameCleanup()
{
	Network::Terminate();
}

void Host()
{
	isHost = true;
	Network::SocketAddress addr(8888);
	listenSocket.Open();
	listenSocket.Bind(addr);
	listenSocket.Listen();

	otherSocket = listenSocket.Accept(clientAddress);


	recvThread = std::thread([]()
	{

		while (true)
		{
			char buffer[1024];
			int bytesReceived = otherSocket->Receive(buffer, std::size(buffer));
			if (bytesReceived > 0 && bytesReceived == sizeof(Runner))
			{
				std::lock_guard<std::mutex> lock(mutex);
				memcpy_s(&runners[1], sizeof(Runner), buffer, bytesReceived);
			}
		}
	}
	);
}

void Join(const char* hostip)
{
	otherSocket = new Network::TCPSocket();
	otherSocket->Connect(Network::SocketAddress(hostip, 8888));

	recvThread = std::thread([]
	{
		while (true)
		{
			char buffer[1024];
			int bytesReceived = otherSocket->Receive(buffer, std::size(buffer));
			if (bytesReceived > 0 && bytesReceived == sizeof(Runner))
			{
				std::lock_guard<std::mutex> lock(mutex);
				memcpy_s(&runners[0], sizeof(Runner), buffer, bytesReceived);
			}
		}
	});
}
void UpdateNetwork()
{

}

bool GameLoop(float deltaTime)
{
	if (!gameStarted)
	{
		if (X::IsKeyPressed(X::Keys::SPACE))
		{
			gameStarted = true;
			Join("127.0.0.1");
		}
		else if (X::IsKeyPressed(X::Keys::H))
		{
			gameStarted = true;
			Host();
		}
	}
	else
	{
		auto& runner = isHost ? runners[0] : runners[1];
		UpdateNetwork();
		if (runner.frameId == 0)
		{
			if (X::IsKeyPressed(X::Keys::RIGHT))
			{
				runner.frameId = 1;
				runner.position.x += 2.0f;
				{
					std::lock_guard<std::mutex> lock(mutex);
					otherSocket->Send(&runner, sizeof(Runner));
				}
			}
		}
		else
		{
			if (X::IsKeyPressed(X::Keys::LEFT))
			{
				runner.frameId = 0;
				runner.position.x += 2.0f;
			}
			{
				std::lock_guard<std::mutex> lock(mutex);
				otherSocket->Send(&runner, sizeof(Runner));
			}
		}
		for (auto& r : runners)
		{
			X::DrawSprite(textureIds[r.frameId], r.position);
		}
	}
	return X::IsKeyPressed(X::Keys::ESCAPE);
}
int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR cmd, int)
{
	X::Start("xconfig.json");
	GameInit();

	X::Run(GameLoop);

	GameCleanup();
	X::Stop();
}