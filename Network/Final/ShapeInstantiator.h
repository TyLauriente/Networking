#pragma once

#include "TytrisTile.h"

const int ROWS = 10;
const int COLLUMNS = 20;
const int BUFFER_SIZE = 4;
const int BUFFER_START = 3;

enum class Shapes
{

	LeftGun, // DarkBlue
	Straight, // LightBlue
	RightSnake, // Green
	Block, // Yellow
	RightGun, // Orange
	LeftSnake, // Red
	Pyramid // Purple
};

class ShapeInstantiator
{
public:
	ShapeInstantiator(std::vector<std::vector<TytrisTile>>& tileGrid);

	void XInitialize();
	
	void Update(bool tickDown);

	X::Math::Vector2 InstanciateShape(Shapes shape);

	bool CanSpawnShapes() const { return m_canSpawnShape; }

private:
	std::vector<std::vector<TytrisTile>>& m_tileGrid;
	TytrisTile m_shapeBuffer[4][4];
	uint8_t m_ticks{ 0 };
	bool m_canSpawnShape{ true };
	
	
	void ClearShapeBuffer();
	bool PushBottomBufferRowToGrid();
	void TickBufferDown();
};

