#pragma once

#include "core/environment.h"

struct Velocity : public Component<Velocity>
{
	Velocity(float speed, float vrot = 0.f, float vscale = 0.f);

	float speed;
	float vrot;
	float vscale;
};
