#include <iostream>
#include "systems/damage_system.h"
#include "Components.h"
#include "events.h"

void DamageSystem::update(Environment* env)
{
	auto collisions = env->getEvents<CollisionEvent>();

	for(unsigned i=0;i<collisions.size();i++)
	{
		handleProjectiles(env, collisions[i]);
	}
}

void DamageSystem::handleProjectiles(Environment* env, const CollisionEvent& collision)
{
	auto projectile = env->get<Projectile>();
	auto health = env->get<Health>();

	unsigned projectile_id = 0;
	unsigned target_id = 0;

	if(env->hasComponents<Projectile>(collision.entity1))
	{
		projectile_id = collision.entity1;
		target_id = collision.entity2;
	}

	if(env->hasComponents<Projectile>(collision.entity2))
	{
		projectile_id = collision.entity2;
		target_id = collision.entity1;
	}

	if(projectile_id != 0)
	{
		if(projectile[projectile_id].shooting_entity != target_id)
		{
			if(env->hasComponents<Solid>(target_id))
			{
				if(env->hasComponents<Health>(target_id))
				{
					int damage = projectile[projectile_id].damage;
					health[target_id].addHealth(-damage);

					if(health[target_id].hasHealth())
					{
						std::cout<<" *hits*"<<std::endl;
					}

					else
					{
						std::cout<<" *BOOM*"<<std::endl;
						
						removeFocus(env, target_id);
						env->destroyEntity(target_id);
					}
				}
				
				env->destroyEntity(projectile_id);
			}
		}
	}
}

void DamageSystem::removeFocus(Environment* env, unsigned target_id)
{
	auto view_controller = env->get<ViewController>();
	for(unsigned i=0;i<env->maxEntities();i++)
	{
		if(env->hasComponents<ViewController>(i))
		{
			std::vector<unsigned>& focuses = view_controller[i].focusedObjects;
			for(unsigned j=0;j<focuses.size();j++)
			{
				if(focuses[j] == target_id)
				{
					focuses.erase(focuses.begin()+j);
					break;
				}
			}
			break;
		}
	}
}
