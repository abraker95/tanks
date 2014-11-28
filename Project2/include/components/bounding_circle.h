#pragma once

#include "core/environment.h"

struct BoundingCircle : public Component<BoundingCircle>
{
	BoundingCircle();
	BoundingCircle(float radius);

	float radius;
	bool collision;
};
