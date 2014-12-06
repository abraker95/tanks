#include "systems/expiring_system.h"

ExpiringSystem::ExpiringSystem()
{
}

ExpiringSystem::~ExpiringSystem()
{
}

void ExpiringSystem::update(Environment* env, float dt)
{
	auto expires = env->get<Expires>();

	for(unsigned i=0;i<env->maxEntities();i++)
	{
		if(env->hasComponents<Expires>(i))
		{
			if(expires[i].time !=-100000)
			{
				expires[i].time -= dt;
				if(expires[i].time<=0.f)
				{
					env->destroyEntity(i);
					break;  // avoid going into the conditional destroy
				}
					
			}
			
		    if(expires[i].condition==true)
			   env->destroyEntity(i);
		}
	}
}
