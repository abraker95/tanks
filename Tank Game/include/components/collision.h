#pragma once
#include "core/environment.h"

struct Collision : public Component<Collision>
{
	Collision();

	unsigned colliding_with;
};
