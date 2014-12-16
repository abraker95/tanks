#pragma once

#include "core/environment.h"
#include "events.h"

class DamageSystem
{
public:
	void update(Environment* env);

private:
	void handleProjectiles(Environment* env, const CollisionEvent& collision);
};
