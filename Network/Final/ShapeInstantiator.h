#pragma once

#include "TytrisTile.h"

constexpr int ROWS = 10;
constexpr int COLLUMNS = 20;
constexpr int BUFFER_SIZE = 4;
constexpr int BUFFER_START = 3;

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

struct GridPosition
{
	GridPosition() = default;
	GridPosition(uint8_t _y, uint8_t _x) : y{ _y }, x{ _x } {}
	bool operator==(const GridPosition& other) { return (x == other.x && y == other.y); }

	uint8_t y{ 0 };
	uint8_t x{ 0 };
};


class ShapeInstantiator
{
public:
	ShapeInstantiator(std::vector<std::vector<TytrisTile>>& tileGrid, bool& pushedToBoard);

	void XInitialize();
	
	void Update(bool tickDown);

	void InstanciateShape(Shapes shape);

	bool CanSpawnShapes() const { return m_canSpawnShape; }

private:
	std::vector<std::vector<TytrisTile>>& m_tileGrid;
	bool& m_shapePushedToBoard;
	TytrisTile m_shapeBuffer[4][4];
	uint8_t m_ticks{ 0 };
	bool m_canSpawnShape{ true };
	
	void ClearShapeBuffer();
	bool PushBottomBufferRowToGrid();
	void TickBufferDown();
};

