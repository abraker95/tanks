#pragma once
#include "core/environment.h"
#include "include/math.h"

struct BoundingBox : public Component<BoundingBox>
{
	BoundingBox(Vec2f size);

	Vec2f size;
};
