#include <XEngine.h>
#include <Network.h>
#include "Ball.h"


//Consts
const float DEFAULT_GRAVITY_FORCE = 7000.0f;

//Forward Decleration
void XInitialize();
void XTerminate();

float GravityForce{ DEFAULT_GRAVITY_FORCE };
Ball ball(GravityForce, {0.0f, 0.0});


bool Update(float deltaTime)
{
	ball.Update(deltaTime);


	ball.Render();

	return X::IsKeyPressed(X::Keys::ESCAPE);
}


int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR , int)
{
	X::Start(/*"XConfig.h"*/);
	XInitialize();


	X::Run(Update);

	XTerminate();
	X::Stop();
}

void XInitialize()
{
	ball.XInitialize();
}

void XTerminate()
{

}