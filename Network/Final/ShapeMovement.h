#pragma once

#include <XEngine.h>
#include "TytrisTile.h"

class ShapeMovement
{
public:
	
	void TickDown(std::vector<std::vector<TytrisTile>>& tileGrid);

	void RotateLeft(std::vector<std::vector<TytrisTile>>& tileGrid);

	void RotateRight(std::vector<std::vector<TytrisTile>>& tileGrid);
};

