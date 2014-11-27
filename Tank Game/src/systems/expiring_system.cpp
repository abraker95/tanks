#include "systems/expiring_system.h"

ExpiringSystem::ExpiringSystem()
{
}

ExpiringSystem::~ExpiringSystem()
{
}

void ExpiringSystem::update(Environment* env, float dt)
{
	Expires* expires = env->get<Expires>();

	for(unsigned i=0;i<env->maxEntities();i++)
	{
		if(env->hasComponents<Expires>(i))
		{
			expires[i].time -= dt;
			if(expires[i].time <= 0.f)
				env->destroyEntity(i);
		}
	}
}
