#pragma once

#include <XEngine.h>
#include "TytrisTile.h"
#include "ShapeInstantiator.h"

class ShapeMovement
{
public:
	
	bool TickDown(std::vector<std::vector<TytrisTile>>& tileGrid, std::vector<GridPosition>& shape);

	void MoveLeft(std::vector<std::vector<TytrisTile>>& tileGrid, std::vector<GridPosition>& shape);

	void MoveRight(std::vector<std::vector<TytrisTile>>& tileGrid, std::vector<GridPosition>& shape);

	void RotateLeft(std::vector<std::vector<TytrisTile>>& tileGrid, std::vector<GridPosition>& shape);

	void RotateRight(std::vector<std::vector<TytrisTile>>& tileGrid, std::vector<GridPosition>& shape);

	bool CanTickDown(std::vector<std::vector<TytrisTile>>& tileGrid, std::vector<GridPosition> shape);
	void SwapTiles(std::vector<std::vector<TytrisTile>>& tileGrid, GridPosition first, GridPosition second);
private:
	bool CanMoveLeft(std::vector<std::vector<TytrisTile>>& tileGrid, std::vector<GridPosition> shape);
	bool CanMoveRight(std::vector<std::vector<TytrisTile>>& tileGrid, std::vector<GridPosition> shape);

};

