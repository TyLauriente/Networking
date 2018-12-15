#pragma once

#include <XEngine.h>
#include "ShapeInstantiator.h"

class BoardUI
{
public:
	void XInitialize(X::Math::Vector2 holdPos);

	void SetHoldBlock(Shapes shape);

	void SetNextBlock(Shapes shape);

	void Render();


private:
	X::TextureId m_shapeTextures[7];
	int m_currentHold{ -1 };
	int m_nextBlock{ -1 };
	X::Math::Vector2 m_holdPosition;
};

