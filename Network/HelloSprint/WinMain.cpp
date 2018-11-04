#include <Network.h>
#include <XEngine.h>

struct Runner
{
	int frameId;
	float position;
};
std::mutex mutex;
std::thread recvThread;
Network::TCPSocket listenSocket;
Network::TCPSocket *otherSocket = nullptr;
Network::SocketAddress clientAddress;

X::TextureId textureIds[2];
std::vector<Runner> runners;
bool isHost = false;

void GameInit()
{
	Network::Initialize();
	textureIds[0] = X::LoadTexture("mario_walk_01.png");
	textureIds[1] = X::LoadTexture("mario_walk_02.png");
	runners.push_back(Runner());
	runners.back().frameId = 0;
	runners.back().position = 100.0f;
}

void GameCleanup()
{
	Network::Terminate();
}

void Host()
{
	isHost = true;
	Network::SocketAddress addr(8888);
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
void UpdateNetwork()
{

}

bool GameLoop(float deltaTime)
{
	auto& runner = isHost ? runners[0] : runners[1];
	UpdateNetwork();
	if (runners[0].frameId == 0)
	{
		if (X::IsKeyPressed(X::Keys::RIGHT))
		{
			runners[0].frameId = 1;
			runners[0].position += 2.0f;
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
			runners[0].frameId = 0;
			runners[0].position += 2.0f;
		}
	}
	for (auto& runner : runners)
	{
		X::DrawSprite(textureIds[runner.frameId], { runner.position,200.0f });
	}
	return X::IsKeyPressed(X::Keys::ESCAPE);
}
int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR cmd, int)
{
	X::Start("xconfig.json");
	GameInit();
	if (cmd != nullptr)
	{
		Join(cmd);
	}
	else
	{
		Host();
	}
	X::Run(GameLoop);

	GameCleanup();
	X::Stop();
}