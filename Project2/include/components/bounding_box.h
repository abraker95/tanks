#pragma once
#include "core/environment.h"

struct BoundingBox : public Component<BoundingBox>
{
	BoundingBox(int width, int height);

	float width, height;
};
