#pragma once

#include <XEngine.h>

const int AMOUNT_OF_TILE_COLORS = 7;


enum class Colors
{
	DarkBlue,
	LightBlue,
	Green,
	Yellow,
	Orange,
	Red,
	Purple,
};

class TytrisTile
{
public:
	TytrisTile();
	
	void XInitialize();

	void Render() { if (m_on) { X::DrawSprite(m_tiles[static_cast<int>(m_currentColor)], m_position); } }

	void SetPoition(X::Math::Vector2 newPosition) { m_position = newPosition; }

	void SetColor(Colors color) { m_currentColor = color; }

	float GetLength() const { return m_tileLength; }

	void TurnOn(bool on) { m_on = on; }

	bool IsOn() const { return m_on; }

	X::Math::Vector2 GetPosition() const { return m_position; }

	Colors GetColor() const { return m_currentColor; }

private:
	float m_tileLength;

	bool m_on{ false };
	X::Math::Vector2 m_position;
	X::TextureId m_tiles[AMOUNT_OF_TILE_COLORS];
	Colors m_currentColor;

};

