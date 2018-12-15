#include "ShapeInstantiator.h"



ShapeInstantiator::ShapeInstantiator(std::vector<std::vector<TytrisTile>>& tileGrid, bool& pushedToBoard)
	: m_tileGrid{ &tileGrid }
	, m_shapePushedToBoard{ &pushedToBoard }
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


void ShapeInstantiator::Update(bool tickDown, std::vector<std::vector<TytrisTile>>& tileGrid, bool& pushedToBoard, bool& dead)
{
	m_tileGrid = &tileGrid;
	m_shapePushedToBoard = &pushedToBoard;
	if (tickDown)
	{
		if (m_ticks > 0)
		{
			m_ticks--;
			PushBottomBufferRowToGrid(dead);
			TickBufferDown();
		}
	}
}

std::vector<GridPosition> ShapeInstantiator::InstanciateShape(Shapes shape)
{
	ClearShapeBuffer();
	m_ticks = 2;

	std::vector<GridPosition> shapeBoardPositions;

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

		shapeBoardPositions.push_back({ BUFFER_START, 0 });
		shapeBoardPositions.push_back({ BUFFER_START, 1 });
		shapeBoardPositions.push_back({ BUFFER_START + 1, 1 });
		shapeBoardPositions.push_back({ BUFFER_START + 2, 1 });
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

		shapeBoardPositions.push_back({ BUFFER_START, 1});
		shapeBoardPositions.push_back({ BUFFER_START + 1, 1 });
		shapeBoardPositions.push_back({ BUFFER_START + 2, 1 });
		shapeBoardPositions.push_back({ BUFFER_START + 3, 1 });
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

		shapeBoardPositions.push_back({ BUFFER_START, 1 });
		shapeBoardPositions.push_back({ BUFFER_START + 1, 1 });
		shapeBoardPositions.push_back({ BUFFER_START + 1, 0 });
		shapeBoardPositions.push_back({ BUFFER_START + 2, 0 });
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

		shapeBoardPositions.push_back({ BUFFER_START + 1, 0 });
		shapeBoardPositions.push_back({ BUFFER_START + 1, 1 });
		shapeBoardPositions.push_back({ BUFFER_START + 2, 0 });
		shapeBoardPositions.push_back({ BUFFER_START + 2, 1 });
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

		shapeBoardPositions.push_back({ BUFFER_START, 1 });
		shapeBoardPositions.push_back({ BUFFER_START + 1, 1 });
		shapeBoardPositions.push_back({ BUFFER_START + 2, 1 });
		shapeBoardPositions.push_back({ BUFFER_START + 2, 0 });

	}
	else if (shape == Shapes::LeftSnake)
	{
		m_shapeBuffer[0][2].TurnOn(true);
		m_shapeBuffer[0][2].SetColor(static_cast<Colors>(shape));

		m_shapeBuffer[1][2].TurnOn(true);
		m_shapeBuffer[1][2].SetColor(static_cast<Colors>(shape));

		m_shapeBuffer[1][3].TurnOn(true);
		m_shapeBuffer[1][3].SetColor(static_cast<Colors>(shape));

		m_shapeBuffer[2][3].TurnOn(true);
		m_shapeBuffer[2][3].SetColor(static_cast<Colors>(shape));

		shapeBoardPositions.push_back({ BUFFER_START, 0 });
		shapeBoardPositions.push_back({ BUFFER_START + 1, 0 });
		shapeBoardPositions.push_back({ BUFFER_START + 1, 1 });
		shapeBoardPositions.push_back({ BUFFER_START + 2, 1 });
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

		shapeBoardPositions.push_back({ BUFFER_START, 1 });
		shapeBoardPositions.push_back({ BUFFER_START + 1, 1 });
		shapeBoardPositions.push_back({ BUFFER_START + 1, 0 });
		shapeBoardPositions.push_back({ BUFFER_START + 2, 1 });
	}
	return shapeBoardPositions;
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

bool ShapeInstantiator::PushBottomBufferRowToGrid(bool& dead)
{
	TytrisTile newTopRow[BUFFER_SIZE];
	if (m_ticks == 0)
	{
		for (uint8_t index = BUFFER_START; index < BUFFER_START + BUFFER_SIZE; ++index)
		{
			GridPosition first{ index, 0 };
			GridPosition second{ index, 1 };
			bool firstOn = (*m_tileGrid)[first.y][first.x].IsOn();
			(*m_tileGrid)[first.y][first.x].TurnOn((*m_tileGrid)[second.y][second.x].IsOn());
			(*m_tileGrid)[second.y][second.x].TurnOn(firstOn);
			Colors firstColor = (*m_tileGrid)[first.y][first.x].GetColor();
			(*m_tileGrid)[first.y][first.x].SetColor((*m_tileGrid)[second.y][second.x].GetColor());
			(*m_tileGrid)[second.y][second.x].SetColor(firstColor);
		}
	}
	for (uint8_t index = 0; index < BUFFER_SIZE; ++index)
	{
		if ((*m_tileGrid)[BUFFER_START + index][0].IsOn())
		{
			dead = true;
			return false;
		}
		X::Math::Vector2 pos = (*m_tileGrid)[BUFFER_START + index][0].GetPosition();
		(*m_tileGrid)[BUFFER_START + index][0] = m_shapeBuffer[index][BUFFER_SIZE - 1];
		(*m_tileGrid)[BUFFER_START + index][0].SetPoition(pos);
		m_shapeBuffer[index][BUFFER_SIZE - 1].TurnOn(false);
	}
	if (m_ticks == static_cast<uint8_t>(0))
	{
		*m_shapePushedToBoard = true;
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