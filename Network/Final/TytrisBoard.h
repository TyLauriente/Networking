#pragma once

#include <XEngine.h>
#include "TytrisTile.h"
#include "ShapeInstantiator.h"

const float FIRST_TILE_X_OFFSET = 69.0f;
const float FIRST_TILE_Y_OFFSET = 33.0f;
const float TICK_WAIT_TIME = 0.75f;


class TytrisBoard
{
public:
	TytrisBoard();
	
	void XInitialize();

	void Update(float deltaTime);

	void Render();

private:
	float ScreenHeight;
	float ScreenWidth;
	float BoardWidth;
	float BoardHeight;
	

	X::Math::Vector2 m_currentHead;
	X::Math::Vector2 m_tytrisBoardPostion;
	X::TextureId m_tytrisBoardTexture;
	std::vector<std::vector<TytrisTile>> m_tileGrid;
	ShapeInstantiator m_shapeInstantiator;
	float m_tickTimer{ 0.0f };
	
	void TickDown();
};

