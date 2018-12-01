#include "TytrisBoard.h"
#include <algorithm>


TytrisBoard::TytrisBoard()
	: m_shapeInstantiator{ m_tileGrid, m_shapePushedToBoard }
{

}


void TytrisBoard::XInitialize()
{
	m_tytrisBoardTexture = X::LoadTexture("TytrisBoard.png");
	ScreenHeight = static_cast<float>(X::GetScreenHeight());
	ScreenWidth = static_cast<float>(X::GetScreenWidth());
	BoardWidth = static_cast<float>(X::GetSpriteWidth(m_tytrisBoardTexture));
	BoardHeight = static_cast<float>(X::GetSpriteHeight(m_tytrisBoardTexture));

	m_tytrisBoardPostion = { ScreenWidth * 0.5f, ScreenHeight * 0.5f };

	X::Math::Vector2 position;
	position.x = (m_tytrisBoardPostion.x - (BoardWidth * 0.5f)) + FIRST_TILE_X_OFFSET;
	position.y = (m_tytrisBoardPostion.y - (BoardHeight * 0.5f)) + FIRST_TILE_Y_OFFSET;

	m_tileGrid.resize(ROWS);
	for (uint8_t y = 0; y < ROWS; ++y)
	{
		m_tileGrid.push_back(std::vector<TytrisTile>());
		for (uint8_t x = 0; x < COLLUMNS; ++x)
		{
			m_tileGrid[y].resize(COLLUMNS);
			m_tileGrid[y].push_back(TytrisTile());
		}
	}

	for (uint8_t y = 0; y < ROWS; ++y)
	{
		for (uint8_t x = 0; x < COLLUMNS; ++x)
		{
			m_tileGrid[y][x].XInitialize();
			m_tileGrid[y][x].SetPoition(position);
			position.y += m_tileGrid[0][0].GetLength();
		}
		position.x += m_tileGrid[0][0].GetLength();
		position.y = (m_tytrisBoardPostion.y - (BoardHeight * 0.5f)) + FIRST_TILE_Y_OFFSET;
	}
	m_shapeInstantiator.XInitialize();
}

void TytrisBoard::Update(float deltaTime)
{
	m_tickTimer += deltaTime;
	if (m_tickTimer >= TICK_WAIT_TIME)
	{
		m_tickTimer = 0.0f;
		TickDown();
 		m_shapeInstantiator.Update(true);
		if (m_shapePushedToBoard)
		{
			m_shapePushedToBoard = false;
			m_canMoveShape = true;
		}
	}

	if (X::IsKeyPressed(X::Keys::SPACE))
	{
		ResetCurrentShape();
		m_shapeInstantiator.InstanciateShape(static_cast<Shapes>(rand() % 7));
		m_canMoveShape = false;
	}
	if (X::IsKeyPressed(X::Keys::LEFT))
	{
		MoveShapeLeft();
	}

}

void TytrisBoard::Render()
{

	X::DrawSprite(m_tytrisBoardTexture, m_tytrisBoardPostion);
	for (uint8_t y = 0; y < ROWS; ++y)
	{
		for (uint8_t x = 0; x < COLLUMNS; ++x)
		{
			m_tileGrid[y][x].Render();
		}
	}
}

void TytrisBoard::TickDown()
{
	if (CanTickDown())
	{
		for (auto it = m_currentShape.rbegin(); it != m_currentShape.rend(); ++it)
		{
			if (m_tileGrid[(*it).y][(*it).x].IsOn())
			{
				SwapTiles((*it), GridPosition{ (*it).y,
					static_cast<uint8_t>((*it).x + 1) });
			}
		}
	}
}

bool TytrisBoard::CanTickDown()
{
	bool canTick{ true };

	for (const auto& pos : m_currentShape)
	{
		if (m_tileGrid[pos.y][pos.x].IsOn() && pos.x == COLLUMNS - 1)
		{
			canTick = false;
			break;
		}
		if (!(std::find(m_currentShape.begin(), m_currentShape.end(),
			GridPosition{ pos.y, static_cast<uint8_t>(pos.x + 1) }) != m_currentShape.end()))
		{
			if (m_tileGrid[pos.y][pos.x + 1].IsOn())
			{
				canTick = false;
				break;
			}
		}
	}
	return canTick;
}

void TytrisBoard::SwapTiles(GridPosition& first, GridPosition second)
{
	m_tileGrid[first.y][first.x].TurnOn(false);
	m_tileGrid[second.y][second.x].SetColor(m_tileGrid[first.y][first.x].GetColor());
	m_tileGrid[second.y][second.x].TurnOn(true);
	
	if (std::find(m_currentShape.begin(), m_currentShape.end(),
		GridPosition{ first.y, static_cast<uint8_t>(first.x + 1) })
		!= m_currentShape.end())
	{
		for (auto& pos : m_currentShape)
		{
			if (pos == second)
			{
				pos = GridPosition{ second.y, second.x };
			}
		}
	}
	else
	{
		first = GridPosition{ second.y, second.x };
	}
}

void TytrisBoard::ResetCurrentShape()
{
	m_currentShape.clear();
	for (uint8_t y = BUFFER_START; y < BUFFER_START + BUFFER_SIZE; ++y)
	{
		for (uint8_t x = 0; x < BUFFER_SIZE - 1; ++x)
		{
			m_currentShape.push_back({ static_cast<uint8_t>(y),
				static_cast<uint8_t>(x) });
		}
	}
}

void TytrisBoard::MoveShapeLeft()
{
	bool canMoveLeft{ true };
	for (const auto& pos : m_currentShape)
	{
		if (pos.y == 0 && m_tileGrid[pos.y][pos.x].IsOn())
		{
			canMoveLeft = false;
		}
	}
	if (canMoveLeft)
	{
		for (auto& pos : m_currentShape)
		{
			SwapTiles(pos, GridPosition{ static_cast<uint8_t>(pos.y - 1), pos.x });
		}
	}
}

void TytrisBoard::MoveShapeRight()
{

}