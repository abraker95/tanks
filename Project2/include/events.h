#pragma once

#include "core/environment.h"

struct CollisionEvent : public Event<CollisionEvent>
{
	CollisionEvent(unsigned entity1, unsigned entity2) : entity1(entity1), entity2(entity2) {}

	unsigned entity1;
	unsigned entity2;
};
