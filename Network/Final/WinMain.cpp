#include <XEngine.h>
#include <Network.h>
#include "TytrisBoard.h"

void XInitialize();
void XTerminate();

TytrisBoard board;

bool Update(float deltaTime)
{
	board.Update(deltaTime);
	board.Render();
	return X::IsKeyPressed(X::Keys::ESCAPE);
}


int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR , int)
{
	X::Start("XConfig.h");
	XInitialize();


	X::Run(Update);

	XTerminate();
	X::Stop();
}

void XInitialize()
{
	board.XInitialize();
}

void XTerminate()
{

}