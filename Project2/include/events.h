#pragma once

#include "core/environment.h"

struct CollisionEvent : public Event<CollisionEvent>
{
	unsigned entity1;
	unsigned entity2;
};
