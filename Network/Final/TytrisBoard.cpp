#include "TytrisBoard.h"
#include <algorithm>


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
	m_currentShape = m_shapeInstantiator.InstanciateShape(static_cast<Shapes>(rand() % 7));
}

void TytrisBoard::Update(float deltaTime)
{
	if (!m_initialized)
	{
		return;
	}
	m_tickTimer += deltaTime;
	if (m_tickTimer >= TICK_WAIT_TIME)
	{
		m_tickTimer = 0.0f;
		if (m_canMoveShape)
		{
			if (!m_shapeMovment.TickDown(m_tileGrid, m_currentShape))
			{
				m_currentShape = m_shapeInstantiator.InstanciateShape(static_cast<Shapes>(rand() % 7));
				m_canMoveShape = false;
			}
		}
  		m_shapeInstantiator.Update(true);
		if (m_shapePushedToBoard)
		{
			m_shapePushedToBoard = false;
			m_canMoveShape = true;
		}
		m_needsNetworkPush = true;
	}

	if (m_canMoveShape)
	{
		if (X::IsKeyPressed(X::Keys::LEFT))
		{
			m_shapeMovment.MoveLeft(m_tileGrid, m_currentShape);
		}
		else if (X::IsKeyPressed(X::Keys::RIGHT))
		{
			m_shapeMovment.MoveRight(m_tileGrid, m_currentShape);
		}
		else if (X::IsKeyPressed(X::Keys::UP))
		{
			m_shapeMovment.RotateLeft(m_tileGrid, m_currentShape);
		}
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

void TytrisBoard::Serialize(Network::StreamWriter& writer) const
{
	for (auto& row : m_tileGrid)
	{
		for (auto& collumn : row)
		{
			collumn.Serialize(writer);
		}
	}
}

void TytrisBoard::Deserialzie(Network::StreamReader& reader)
{
	for (auto& row : m_tileGrid)
	{
		for (auto& collumn : row)
		{
			collumn.Deserialize(reader);
		}
	}
}