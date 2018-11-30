#include "TytrisTile.h"



TytrisTile::TytrisTile()
{
}


void TytrisTile::XInitialize()
{
	m_tiles[0] = X::LoadTexture("DarkBlueTile.png");
	m_tiles[1] = X::LoadTexture("LightBlueTile.png");
	m_tiles[2] = X::LoadTexture("GreenTile.png");
	m_tiles[3] = X::LoadTexture("YellowTile.png");
	m_tiles[4] = X::LoadTexture("OrangeTile.png");
	m_tiles[5] = X::LoadTexture("RedTile.png");
	m_tiles[6] = X::LoadTexture("PurpleTile.png");

	m_tileLength = X::GetSpriteWidth(m_tiles[0]);
	m_currentColor = Colors::DarkBlue;
}