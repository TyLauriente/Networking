#pragma once

#include <XEngine.h>
#include "TytrisTile.h"
#include "ShapeInstantiator.h"
#include "ShapeMovement.h"
#include "Command.h"
#include <Network.h>

constexpr float FIRST_TILE_X_OFFSET = 69.0f;
constexpr float FIRST_TILE_Y_OFFSET = 33.0f;
constexpr float TICK_WAIT_TIME = 0.2f;



class TytrisBoard
{
public:
	TytrisBoard();
	
	void XInitialize();

	void Update(float deltaTime);

	void Render();


	bool NeedsNetworkPush() const { return m_needsNetworkPush; }
	void Push() { m_needsNetworkPush = false; }
	void Dirty() { m_needsNetworkPush = true; }

	void SetPosition(X::Math::Vector2 newPosition) { m_tytrisBoardPostion = newPosition; }
	X::Math::Vector2 GetPosition() const { return m_tytrisBoardPostion; }

	bool Initialized() const { return m_initialized; }

	Shapes GetShapeToSpawn() const { return m_shapeToSpawn; }

	bool SetBoardCommand(BoardCommand command);
	
	bool CanMoveShape() const { return m_canMoveShape; }

	bool CanSpawnShapes() const { return m_canSpawnShapes; }
	void SetCanSpawnShapes(bool spawn) { m_canSpawnShapes = spawn; }

	void SpawnShape(Shapes shape) { m_currentShape = m_shapeInstantiator.InstanciateShape(shape);
																		m_canMoveShape = false; }

private:
	float ScreenHeight;
	float ScreenWidth;
	float BoardWidth;
	float BoardHeight;
	bool m_needsNetworkPush{ false };
	bool m_initialized{ false };
	bool m_canSpawnShapes{ true };
	
	std::vector<GridPosition> m_currentShape;
	Shapes m_shapeToSpawn;
	bool m_shapePushedToBoard{ false };
	bool m_canMoveShape{ false };
	X::Math::Vector2 m_tytrisBoardPostion;
	X::TextureId m_tytrisBoardTexture;
	std::vector<std::vector<TytrisTile>> m_tileGrid;
	ShapeInstantiator m_shapeInstantiator;
	ShapeMovement m_shapeMovment;
	float m_tickTimer{ 0.0f };
};

