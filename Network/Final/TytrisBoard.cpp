#include "TytrisBoard.h"
#include <algorithm>
#include "Command.h"


TytrisBoard::TytrisBoard()
	: m_shapeInstantiator{ m_tileGrid, m_shapePushedToBoard }
{

}


void TytrisBoard::XInitialize()
{
	m_initialized = true;
	m_tytrisBoardTexture = X::LoadTexture("TytrisBoard.png");
	ScreenHeight = static_cast<float>(X::GetScreenHeight());
	ScreenWidth = static_cast<float>(X::GetScreenWidth());
	BoardWidth = static_cast<float>(X::GetSpriteWidth(m_tytrisBoardTexture));
	BoardHeight = static_cast<float>(X::GetSpriteHeight(m_tytrisBoardTexture));

	m_tileGrid.resize(ROWS);
	for (uint8_t y = 0; y < ROWS; ++y)
	{
		m_tileGrid[y].resize(COLLUMNS);
	}

	X::Math::Vector2 position;
	position.x = (m_tytrisBoardPostion.x - (BoardWidth * 0.5f)) + FIRST_TILE_X_OFFSET;
	position.y = (m_tytrisBoardPostion.y - (BoardHeight * 0.5f)) + FIRST_TILE_Y_OFFSET;

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
	m_shapeToSpawn = static_cast<Shapes>(rand() % 7);
	m_currentShape = m_shapeInstantiator.InstanciateShape(m_shapeToSpawn);
	Dirty();
}

void TytrisBoard::Update(float deltaTime)
{
	if (!m_initialized)
	{
		return;
	}
	m_tickTimer += deltaTime;
	m_moveTimer += deltaTime;
	if (m_moveTimer >= MOVE_WAIT_TIME)
	{
		m_moveTimer = 0.0f;
		m_moveTimeValid = true;
	}
	if (m_tickTimer >= TICK_WAIT_TIME)
	{
		m_tickTimer = 0.0f;
		if (m_canMoveShape)
		{
			if (m_canSpawnShapes)
			{
				if (!m_shapeMovement.TickDown(m_tileGrid, m_currentShape))
				{
					m_placeTimer += deltaTime;
					m_maxPlaceTimer += deltaTime;
					if (m_placeTimer >= PLACE_WAIT_TIME || m_maxPlaceTimer >= MAX_PLACE_WAIT_TIME)
					{
						m_shapeToSpawn = static_cast<Shapes>(rand() % 7);
						m_currentShape = m_shapeInstantiator.InstanciateShape(m_shapeToSpawn);
						Dirty();
						m_canMoveShape = false;
						m_placeTimer = 0.0f;
						m_maxPlaceTimer = 0.0f;
						AddLines();
					}
				}
			}
			else
			{
				if (!m_shapeMovement.TickDown(m_tileGrid, m_currentShape))
				{
					AddLines();
				}
			}
		}
  		m_shapeInstantiator.Update(true, m_tileGrid, m_shapePushedToBoard, m_dead);
	}
	ClearFullLines();
	if (m_canMoveShape)
	{
		ShowShapeDestination();
	}
	if (m_shapePushedToBoard)
	{
		m_shapePushedToBoard = false;
		m_canMoveShape = true;
	}
}

void TytrisBoard::Render()
{
	if (!m_initialized)
	{
		return;
	}
	X::DrawSprite(m_tytrisBoardTexture, m_tytrisBoardPostion);
	for (uint8_t y = 0; y < ROWS; ++y)
	{
		for (uint8_t x = 0; x < COLLUMNS; ++x)
		{
			m_tileGrid[y][x].Render();
		}
	}
}

bool TytrisBoard::SetBoardCommand(BoardCommand command)
{
	if (m_canMoveShape && m_moveTimeValid)
	{
		m_moveTimeValid = false;
		m_moveTimer = 0.0f;
		m_placeTimer = 0.0f;
		if (command == BoardCommand::MoveLeft)
		{
			m_shapeMovement.MoveLeft(m_tileGrid, m_currentShape);
		}
		else if (command == BoardCommand::MoveRight)
		{
			m_shapeMovement.MoveRight(m_tileGrid, m_currentShape);
		}
		else if (command == BoardCommand::RotateLeft)
		{
			m_shapeMovement.RotateLeft(m_tileGrid, m_currentShape);
		}
		else if (command == BoardCommand::MoveDown)
		{
			m_shapeMovement.TickDown(m_tileGrid, m_currentShape);
		}
		return true;
	}
	return false;
}

void TytrisBoard::ShowShapeDestination()
{
	if (m_currentShapeDestination.size() > 0)
	{
		for (auto& tile : m_currentShapeDestination)
		{
			m_tileGrid[tile.y][tile.x].SetDestination(false);
		}
	}
	m_currentShapeDestination = m_currentShape;
	while (m_shapeMovement.CanTickDown(m_tileGrid, m_currentShapeDestination)) 
	{
		for (auto& tile : m_currentShapeDestination)
		{
			tile.x++;
		}
	}
	for (auto& tile : m_currentShapeDestination)
	{
		if (!m_tileGrid[tile.y][tile.x].IsOn())
		{
			m_tileGrid[tile.y][tile.x].SetDestination(true);
		}
	}
}

void TytrisBoard::ClearFullLines()
{
	for (uint8_t x = 0; x < COLLUMNS; ++x)
	{
		bool clear{ true };
		for (uint8_t y = 0; y < ROWS; ++y)
		{
			if (!m_tileGrid[y][x].IsOn() || (std::find(m_currentShape.begin(), m_currentShape.end(),
				GridPosition{ y, x }) != m_currentShape.end()))
			{
				clear = false;
			}
		}
		if (clear)
		{
			for (uint8_t y = 0; y < ROWS; ++y)
			{
				m_tileGrid[y][x].TurnOn(false);
			}
			MoveLinesDown(x);
			m_clearedLines++;
		}
	}
}

void TytrisBoard::MoveLinesDown(uint8_t row)
{
	for (uint8_t x = row - 1; x > 1; --x)
	{
		for (uint8_t y = 0; y < ROWS; ++y)
		{
			m_shapeMovement.SwapTiles(m_tileGrid, GridPosition{ y, x },
				GridPosition{ y, static_cast<uint8_t>(x + 1) });
		}
	}

}

void TytrisBoard::AddLines()
{
	while (m_linesToAdd > 0)
	{
		for (uint8_t x = 2; x < COLLUMNS; ++x)
		{
			for (uint8_t y = 0; y < ROWS; ++y)
			{
				m_shapeMovement.SwapTiles(m_tileGrid, GridPosition{ y, x },
					GridPosition{ y, static_cast<uint8_t>(x - 1) });
			}
		}
		int openPos = rand() % static_cast<int>(ROWS - 1);
		for (uint8_t y = 0; y < ROWS; ++y)
		{
			m_tileGrid[y][COLLUMNS - 1].SetColor(Colors::DarkBlue);
			if (y == openPos)
			{
				m_tileGrid[y][COLLUMNS - 1].TurnOn(false);
			}
			else
			{
				m_tileGrid[y][COLLUMNS - 1].TurnOn(true);
			}
		}
		m_linesToAdd--;
	}
}