#pragma once

#include <XEngine.h>
#include "TytrisTile.h"

class ShapeMovement
{
public:
	
	void TickDown(std::vector<std::vector<TytrisTile>>& tileGrid, X::Math::Vector2 head);

	void RotateLeft(std::vector<std::vector<TytrisTile>>& tileGrid, X::Math::Vector2 head);

	void RotateRight(std::vector<std::vector<TytrisTile>>& tileGrid, X::Math::Vector2 head);
};

