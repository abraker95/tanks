#pragma once

#include "core/environment.h"

struct Velocity : public Component<Velocity>
{
	Velocity();
	Velocity(float speed, float vrot);

	float speed;
	float vrot;
};
