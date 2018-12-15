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

const char* host = "127.0.0.1";

X::Math::Vector2 backgroundPos;
X::TextureId background_connecting;
X::TextureId background_connected;
X::TextureId background_lose;
X::TextureId background_win;

bool Update(float deltaTime)
{
	if (gameState == GameState::Disconnected)
	{
		X::DrawSprite(background_connecting, backgroundPos);
		if (ClientManager::Get()->ConnectToServer(host, 8888))
		{
			gameState = GameState::Connected;
		}
	}
	else if (gameState == GameState::Connected)
	{
		ClientManager::Get()->HandleMessage();
		ClientManager::Get()->Update(deltaTime);
		X::DrawSprite(background_connected, backgroundPos);
		ClientManager::Get()->Render();
		if (ClientManager::Get()->Won())
		{
			gameState = GameState::WinGame;
		}
		else if (ClientManager::Get()->IsDead())
		{
			gameState = GameState::LoseGame;
		}
	}
	else if (gameState == GameState::LoseGame)
	{
		X::DrawSprite(background_lose, backgroundPos);
	}
	else
	{
		X::DrawSprite(background_win, backgroundPos);
	}
	return false;
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if (lpCmdLine[0] != 0)
	{
		host = lpCmdLine;
	}

	ClientManager::StaticInitialize();
	X::Start("XConfig.json");
	background_connecting = X::LoadTexture("TetrisConnectingBackground.png");
	background_connected = X::LoadTexture("TetrisBackground.png");
	background_lose = X::LoadTexture("TetrisLoseBackground.png");
	background_win = X::LoadTexture("TetrisWinBackground.png");
	backgroundPos.x = (X::GetScreenWidth() * 0.5f);
	backgroundPos.y = (X::GetScreenHeight() * 0.5f);

	X::Run(Update);

	X::Stop();
	ClientManager::StaticTerminate();
}