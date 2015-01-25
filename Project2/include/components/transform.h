#pragma once

#include "core/environment.h"
#include "math/vector.h"

struct Transform : public Component<Transform>
{
	Transform(Vec2f pos, float rot = 0.f, float scale = 1.f);

	Vec2f pos;
	float rot;
	float scale;
};
