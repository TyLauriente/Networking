#pragma once

#include "TytrisTile.h"

constexpr uint8_t ROWS = 10;
constexpr uint8_t COLLUMNS = 20;
constexpr uint8_t BUFFER_SIZE = 4;
constexpr uint8_t BUFFER_START = 3;

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
	GridPosition(uint8_t _y, uint8_t _x, uint8_t _rotation = 0) : y{ _y }, x{ _x }, rotaion{ _rotation } {}
	bool operator==(const GridPosition& other) { return (x == other.x && y == other.y); }

	void Serialize(Network::StreamWriter& writer) const
	{
		writer.Write(y);
		writer.Write(x);
		writer.Write(rotaion);
	}

	void Deserialzie(Network::StreamReader& reader)
	{
		reader.Read(y);
		reader.Read(x);
		reader.Read(rotaion);
	}


	uint8_t y{ 0 };
	uint8_t x{ 0 };
	uint32_t rotaion{ 0 };
};


class ShapeInstantiator
{
public:
	ShapeInstantiator(std::vector<std::vector<TytrisTile>>& tileGrid, bool& pushedToBoard);

	void XInitialize();
	
	void Update(bool tickDown, std::vector<std::vector<TytrisTile>>& tileGrid, bool& pushedToBoard, bool& dead);

	std::vector<GridPosition> InstanciateShape(Shapes shape);

	bool CanSpawnShapes() const { return m_canSpawnShape; }

private:
	std::vector<std::vector<TytrisTile>>* m_tileGrid;
	bool* m_shapePushedToBoard;
	TytrisTile m_shapeBuffer[4][4];
	uint8_t m_ticks{ 0 };
	bool m_canSpawnShape{ true };
	
	void ClearShapeBuffer();
	bool PushBottomBufferRowToGrid(bool& dead);
	void TickBufferDown();
};

