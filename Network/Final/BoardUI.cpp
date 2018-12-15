#include "BoardUI.h"


void BoardUI::XInitialize(X::Math::Vector2 holdPos)
{
	m_holdPosition = holdPos;
	m_shapeTextures[0] = X::LoadTexture("J.png");
	m_shapeTextures[1] = X::LoadTexture("I.png");
	m_shapeTextures[2] = X::LoadTexture("S.png");
	m_shapeTextures[3] = X::LoadTexture("O.png");
	m_shapeTextures[4] = X::LoadTexture("L.png");
	m_shapeTextures[5] = X::LoadTexture("Z.png");
	m_shapeTextures[6] = X::LoadTexture("T.png");
}

void BoardUI::SetHoldBlock(Shapes shape)
{
	m_currentHold = static_cast<int>(shape);
}

void BoardUI::SetNextBlock(Shapes shape)
{
	shape;
}

void BoardUI::Render()
{
	if (m_currentHold >= 0)
	{
		X::DrawSprite(m_shapeTextures[m_currentHold], m_holdPosition);
	}
}
