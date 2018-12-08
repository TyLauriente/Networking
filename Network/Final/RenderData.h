#pragma once

#include <XEngine.h>

struct RenderData
{
	RenderData(X::TextureId texture, X::Math::Vector2 pos)
		: textureId{ texture }
		, position{ pos }
	{}

	X::TextureId textureId;
	X::Math::Vector2 position;
};