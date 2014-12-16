#pragma once
#include "core/environment.h"
#include "math.h"
#include "math/vector.h"

struct BoundingBox : public Component<BoundingBox>
{
	BoundingBox(Vec2f size);

	Vec2f size;
};
