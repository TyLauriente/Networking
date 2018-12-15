#pragma once

#include <XEngine.h>
#include <Network.h>

const int AMOUNT_OF_TILE_COLORS = 8;


enum class Colors
{
	DarkBlue,
	LightBlue,
	Green,
	Yellow,
	Orange,
	Red,
	Purple,
	Grey
};

class TytrisTile
{
public:
	TytrisTile();
	
	void XInitialize();

	void Render();

	void SetPoition(X::Math::Vector2 newPosition) { m_position = newPosition; }

	void SetColor(Colors color) { m_currentColor = color; }

	float GetLength() const { return m_tileLength; }

	void TurnOn(bool on) { m_on = on; m_destination = false; }

	bool IsOn() const { return m_on; }

	void SetDestination(bool destination) { m_destination = destination; }

	X::Math::Vector2 GetPosition() const { return m_position; }

	Colors GetColor() const { return m_currentColor; }

	void Serialize(Network::StreamWriter& writer) const;
	void Deserialize(Network::StreamReader& reader);

private:
	float m_tileLength;
	bool m_needsNetworkUpdate{ false };
	bool m_destination{ false };

	bool m_on{ false };
	X::Math::Vector2 m_position;
	X::TextureId m_tiles[AMOUNT_OF_TILE_COLORS];
	Colors m_currentColor;

};

