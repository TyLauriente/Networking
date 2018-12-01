#pragma once

#include <XEngine.h>
#include "TytrisTile.h"
#include "ShapeInstantiator.h"

constexpr float FIRST_TILE_X_OFFSET = 69.0f;
constexpr float FIRST_TILE_Y_OFFSET = 33.0f;
constexpr float TICK_WAIT_TIME = 0.2f;



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
	
	std::vector<GridPosition> m_currentShape;
	bool m_shapePushedToBoard{ false };
	bool m_canMoveShape{ false };
	X::Math::Vector2 m_tytrisBoardPostion;
	X::TextureId m_tytrisBoardTexture;
	std::vector<std::vector<TytrisTile>> m_tileGrid;
	ShapeInstantiator m_shapeInstantiator;
	float m_tickTimer{ 0.0f };
	
	void TickDown();
	bool CanTickDown();
	void SwapTiles(GridPosition first, GridPosition second);
	void ResetCurrentShape();
	void MoveShapeLeft();
	void MoveShapeRight();
};

