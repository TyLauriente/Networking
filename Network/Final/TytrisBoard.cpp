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
}

void TytrisBoard::Update(float deltaTime)
{
	m_tickTimer += deltaTime;
	if (m_tickTimer >= TICK_WAIT_TIME)
	{
		m_tickTimer = 0.0f;
		if (m_canMoveShape)
		{
			m_shapeMovment.TickDown(m_tileGrid, m_currentShape);
		}
  		m_shapeInstantiator.Update(true);
		if (m_shapePushedToBoard)
		{
			m_shapePushedToBoard = false;
			m_canMoveShape = true;
		}
	}

	if (X::IsKeyPressed(X::Keys::SPACE))
	{
		m_currentShape = m_shapeInstantiator.InstanciateShape(static_cast<Shapes>(0));
		m_canMoveShape = false;
	}
	if (X::IsKeyPressed(X::Keys::LEFT))
	{
		m_shapeMovment.MoveLeft(m_tileGrid, m_currentShape);
	}
	if (X::IsKeyPressed(X::Keys::RIGHT))
	{
		m_shapeMovment.MoveRight(m_tileGrid, m_currentShape);
	}
	if (X::IsKeyPressed(X::Keys::DOWN))
	{
		m_shapeMovment.RotateLeft(m_tileGrid, m_currentShape);
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
