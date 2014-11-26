#pragma once

#include "core/environment.h"

struct Velocity : public Component<Velocity>
{
	Velocity();
	Velocity(float vx, float vy, float vrot);

	float vx, vy;
	float vrot;
};
