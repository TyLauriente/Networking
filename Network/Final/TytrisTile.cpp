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
	m_tiles[7] = X::LoadTexture("DestinationTile.png");

	m_tileLength = static_cast<float>(X::GetSpriteWidth(m_tiles[0]));
	m_currentColor = Colors::DarkBlue;
}

void TytrisTile::Render()
{
	if (m_destination)
	{
		X::DrawSprite(m_tiles[7], m_position);
		return;
	}
	if (m_on) 
	{
		X::DrawSprite(m_tiles[static_cast<int>(m_currentColor)], m_position); 
	}
}

void TytrisTile::Serialize(Network::StreamWriter& writer) const
{
	writer.Write(m_position.x);
	writer.Write(m_position.y);
	writer.Write(m_on);
	int color = static_cast<int>(m_currentColor);
	writer.Write(color);
}

void TytrisTile::Deserialize(Network::StreamReader& reader)
{
	reader.Read(m_position.x);
	reader.Read(m_position.y);
	reader.Read(m_on);
	int color;
	reader.Read(color);
	m_currentColor = static_cast<Colors>(color);
}