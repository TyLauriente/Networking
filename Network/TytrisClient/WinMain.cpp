#include <XEngine.h>
#include <Network.h>
#include <TytrisGame.h>
#include "ClientManager.h"

enum class GameState
{
	Disconnected,
	Connected,
	LoseGame,
	WinGame
};

GameState gameState{ GameState::Disconnected };

bool Update(float deltaTime)
{
	if (gameState == GameState::Disconnected)
	{
		X::DrawScreenText("Disconnected...", 10.0f, 0.0f, 16.0f, X::Math::Vector4::White());
		if (ClientManager::Get()->ConnectToServer("127.0.0.1", 8888))
		{
			gameState = GameState::Connected;
		}
	}
	else if (gameState == GameState::Connected)
	{
		X::DrawScreenText("Connected", 10.0f, 0.0f, 16.0f, X::Math::Vector4::Green());

		ClientManager::Get()->HandleMessage();
		ClientManager::Get()->Update(deltaTime);
		ClientManager::Get()->Render();
	}
	else if (gameState == GameState::LoseGame)
	{

	}
	else
	{

	}
	return false;
}

int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR cmd, int)
{
	ClientManager::StaticInitialize();
	X::Start("XConfig.json");

	X::Run(Update);

	X::Stop();
	ClientManager::StaticTerminate();
}