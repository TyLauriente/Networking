#include<XEngine.h>
#include"ClientNetworkManager.h"

enum class GameState
{
	Connecting,
	Connected,
	Disconnectd
};

GameState gameState = GameState::Connecting;
const char* host = "127.0.0.1";
const uint16_t port = 8888;

void UpdateConnecting()
{
	X::DrawScreenText("Connecting...", 10.0f, 10.0f, 16.0f, X::Math::Vector4::White());
	
	if (ClientNetworkManager::Get()->ConnectToServer(host, port))
	{
		gameState = GameState::Connected;
	}
	else
	{
		gameState = GameState::Disconnectd;
	}
}

void UpdateConnected(float deltaTime)
{
	X::DrawScreenText("Connected", 10.0f, 10.0f, 16.0f, X::Math::Vector4::White());

	if (ClientNetworkManager::Get()->HandleMessage())
	{
		Command command;
		if (X::IsKeyDown(X::Keys::UP))
		{
			command.m_timedCommand |= Command::kUp;
		}
		if (X::IsKeyDown(X::Keys::DOWN))
		{
			command.m_timedCommand |= Command::kDown;
		}
		if (X::IsKeyDown(X::Keys::LEFT))
		{
			command.m_timedCommand |= Command::kLeft;
		}
		if (X::IsKeyDown(X::Keys::RIGHT))
		{
			command.m_timedCommand |= Command::kRight;
		}
		if (!command.Empty())
		{
			command.SetDuration(deltaTime);

			Network::MemoryStream memStream;
			Network::StreamWriter writer(memStream);
			writer.Write(MessageType::Msg_Command);
			command.Serialize(writer);
			ClientNetworkManager::Get()->SendMessageToServer(memStream);
		}

		GameObjectManager::Get()->Update(deltaTime);
		GameObjectManager::Get()->Draw();
	}
	else
	{
		gameState = GameState::Disconnectd;
	}
}

void UpdateDisconnected()
{
	X::DrawScreenText("Disconneccted", 10.0f, 10.0f, 16.0f, X::Math::Vector4::White());
}

bool Update(float deltaTime)
{
	switch (gameState)
	{
	case GameState::Connecting:
		UpdateConnecting();
		break;
	case GameState::Connected:
		UpdateConnected(deltaTime);
		break;
	case GameState::Disconnectd:
		UpdateDisconnected();
		break;
	}
	return X::IsKeyPressed(X::Keys::ESCAPE);
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if (lpCmdLine[0] != 0)
	{
		host = lpCmdLine;
	}

	GameObjectManager::StaticInitialize();
	ClientNetworkManager::StaticInitialize();

	GameObjectManager::Get()->GetFactory().Register<Character>();

	X::Start();

	X::Run(Update);

	X::Stop();

	GameObjectManager::StaticTerminate();
	ClientNetworkManager::StaticTerminate();
}