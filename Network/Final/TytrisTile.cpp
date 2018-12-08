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

	m_tileLength = static_cast<float>(X::GetSpriteWidth(m_tiles[0]));
	m_currentColor = Colors::DarkBlue;
}

void TytrisTile::Serialize(Network::StreamWriter& writer) const
{
	writer.Write(m_on);
	writer.Write(m_currentColor);
}

void TytrisTile::Deserialize(Network::StreamReader& reader)
{
	reader.Read(m_on);
	reader.Read(m_currentColor);
}