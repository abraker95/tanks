#pragma once

#include "core/environment.h"

struct Transform : public Component<Transform>
{
	Transform(float x, float y, float rot);

	float x, y;
	float rot;
};
