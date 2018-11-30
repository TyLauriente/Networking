#include "ShapeInstantiator.h"



ShapeInstantiator::ShapeInstantiator(std::vector<std::vector<TytrisTile>>& tileGrid)
	: m_tileGrid{ tileGrid }
{

}

void ShapeInstantiator::XInitialize()
{
	for (uint8_t y = 0; y < BUFFER_SIZE; ++y)
	{
		for (uint8_t x = 0; x < BUFFER_SIZE; ++x)
		{
			m_shapeBuffer[y][x].XInitialize();
		}
	}
}


void ShapeInstantiator::Update(bool tickDown)
{
	if (tickDown)
	{
		if (m_ticks > 0)
		{
			m_ticks--;
			if (!PushBottomBufferRowToGrid())
			{
				m_canSpawnShape = false;
			}
			TickBufferDown();
		}
	}
}

X::Math::Vector2 ShapeInstantiator::InstanciateShape(Shapes shape)
{
	ClearShapeBuffer();
	m_ticks = 2;

	X::Math::Vector2 head;

	if (shape == Shapes::LeftGun)
	{
		m_shapeBuffer[0][2].TurnOn(true);
		m_shapeBuffer[0][2].SetColor(static_cast<Colors>(shape));

		m_shapeBuffer[0][3].TurnOn(true);
		m_shapeBuffer[0][3].SetColor(static_cast<Colors>(shape));

		m_shapeBuffer[1][3].TurnOn(true);
		m_shapeBuffer[1][3].SetColor(static_cast<Colors>(shape));

		m_shapeBuffer[2][3].TurnOn(true);
		m_shapeBuffer[2][3].SetColor(static_cast<Colors>(shape));

		head = { BUFFER_START, 0 };
	}
	else if (shape == Shapes::Straight)
	{
		m_shapeBuffer[0][3].TurnOn(true);
		m_shapeBuffer[0][3].SetColor(static_cast<Colors>(shape));

		m_shapeBuffer[1][3].TurnOn(true);
		m_shapeBuffer[1][3].SetColor(static_cast<Colors>(shape));

		m_shapeBuffer[2][3].TurnOn(true);
		m_shapeBuffer[2][3].SetColor(static_cast<Colors>(shape));

		m_shapeBuffer[3][3].TurnOn(true);
		m_shapeBuffer[3][3].SetColor(static_cast<Colors>(shape));

		head = { BUFFER_START, 1 };
	}
	else if (shape == Shapes::RightSnake)
	{
		m_shapeBuffer[0][3].TurnOn(true);
		m_shapeBuffer[0][3].SetColor(static_cast<Colors>(shape));

		m_shapeBuffer[1][3].TurnOn(true);
		m_shapeBuffer[1][3].SetColor(static_cast<Colors>(shape));

		m_shapeBuffer[1][2].TurnOn(true);
		m_shapeBuffer[1][2].SetColor(static_cast<Colors>(shape));

		m_shapeBuffer[2][2].TurnOn(true);
		m_shapeBuffer[2][2].SetColor(static_cast<Colors>(shape));

		head = { BUFFER_START, 1 };
	}
	else if (shape == Shapes::Block)
	{
		m_shapeBuffer[1][2].TurnOn(true);
		m_shapeBuffer[1][2].SetColor(static_cast<Colors>(shape));

		m_shapeBuffer[1][3].TurnOn(true);
		m_shapeBuffer[1][3].SetColor(static_cast<Colors>(shape));

		m_shapeBuffer[2][2].TurnOn(true);
		m_shapeBuffer[2][2].SetColor(static_cast<Colors>(shape));

		m_shapeBuffer[2][3].TurnOn(true);
		m_shapeBuffer[2][3].SetColor(static_cast<Colors>(shape));

		head = { BUFFER_START + 1, 0 };
	}
	else if (shape == Shapes::RightGun)
	{
		m_shapeBuffer[0][3].TurnOn(true);
		m_shapeBuffer[0][3].SetColor(static_cast<Colors>(shape));

		m_shapeBuffer[1][3].TurnOn(true);
		m_shapeBuffer[1][3].SetColor(static_cast<Colors>(shape));

		m_shapeBuffer[2][3].TurnOn(true);
		m_shapeBuffer[2][3].SetColor(static_cast<Colors>(shape));

		m_shapeBuffer[2][2].TurnOn(true);
		m_shapeBuffer[2][2].SetColor(static_cast<Colors>(shape));

		head = { BUFFER_START, 1 };
	}
	else if (shape == Shapes::LeftSnake)
	{
		m_shapeBuffer[0][2].TurnOn(true);
		m_shapeBuffer[0][2].SetColor(static_cast<Colors>(shape));

		m_shapeBuffer[0][3].TurnOn(true);
		m_shapeBuffer[0][3].SetColor(static_cast<Colors>(shape));

		m_shapeBuffer[1][3].TurnOn(true);
		m_shapeBuffer[1][3].SetColor(static_cast<Colors>(shape));

		m_shapeBuffer[2][3].TurnOn(true);
		m_shapeBuffer[2][3].SetColor(static_cast<Colors>(shape));

		head = { BUFFER_START, 0 };	
	}
	else if (shape == Shapes::Pyramid)
	{
		m_shapeBuffer[0][3].TurnOn(true);
		m_shapeBuffer[0][3].SetColor(static_cast<Colors>(shape));

		m_shapeBuffer[1][3].TurnOn(true);
		m_shapeBuffer[1][3].SetColor(static_cast<Colors>(shape));

		m_shapeBuffer[1][2].TurnOn(true);
		m_shapeBuffer[1][2].SetColor(static_cast<Colors>(shape));

		m_shapeBuffer[2][3].TurnOn(true);
		m_shapeBuffer[2][3].SetColor(static_cast<Colors>(shape));

		head = { BUFFER_START, 0 };
	}

	return head;
}

void ShapeInstantiator::ClearShapeBuffer()
{
	for (uint8_t y = 0; y < BUFFER_SIZE; ++y)
	{
		for (uint8_t x = 0; x < BUFFER_SIZE; ++x)
		{
			m_shapeBuffer[y][x].TurnOn(false);
		}
	}
}

bool ShapeInstantiator::PushBottomBufferRowToGrid()
{
	TytrisTile newTopRow[BUFFER_SIZE];
	for (uint8_t index = 0; index < BUFFER_SIZE; ++index)
	{
		if (!m_tileGrid[BUFFER_START + index][0].IsOn())
		{
			X::Math::Vector2 pos = m_tileGrid[BUFFER_START + index][0].GetPosition();
			m_tileGrid[BUFFER_START + index][0] = m_shapeBuffer[index][BUFFER_SIZE - 1];
			m_tileGrid[BUFFER_START + index][0].SetPoition(pos);
			m_shapeBuffer[index][BUFFER_SIZE - 1].TurnOn(false);
		}
		else
		{
			return false;
		}
	}
	return true;
}

void ShapeInstantiator::TickBufferDown()
{
	for (uint8_t y = 0; y < BUFFER_SIZE; ++y)
	{
		for (uint8_t x = BUFFER_SIZE - 1; x > 0; --x)
		{
			m_shapeBuffer[y][x] = m_shapeBuffer[y][x - 1];
			m_shapeBuffer[y][x - 1].TurnOn(false);
		}
	}
}