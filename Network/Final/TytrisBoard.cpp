#include "TytrisBoard.h"



TytrisBoard::TytrisBoard()
	: m_shapeInstantiator{ m_tileGrid }
{

}


void TytrisBoard::XInitialize()
{
	m_tytrisBoardTexture = X::LoadTexture("TytrisBoard.png");
	ScreenHeight = X::GetScreenHeight();
	ScreenWidth = X::GetScreenWidth();
	BoardWidth = X::GetSpriteWidth(m_tytrisBoardTexture);
	BoardHeight = X::GetSpriteHeight(m_tytrisBoardTexture);

	m_tytrisBoardPostion = { ScreenWidth * 0.3f, ScreenHeight * 0.5f };

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
	}

	if (X::IsKeyPressed(X::Keys::SPACE))
	{
		m_currentHead = m_shapeInstantiator.InstanciateShape(static_cast<Shapes>(rand() % 7));
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
	for (uint8_t y = 0; y < ROWS - 1; ++y)
	{
		for (uint8_t x = COLLUMNS - 1; x > 0; --x)
		{
			if (m_tileGrid[y][x - 1].IsOn())
			{
				if (!m_tileGrid[y][x].IsOn())
				{
					X::Math::Vector2 pos = m_tileGrid[y][x].GetPosition();
					m_tileGrid[y][x] = m_tileGrid[y][x - 1];
					m_tileGrid[y][x].SetPoition(pos);
					m_tileGrid[y][x - 1].TurnOn(false);
				}
			}
		}
	}
}
