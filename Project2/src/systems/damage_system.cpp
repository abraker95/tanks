#include <iostream>
#include "systems/damage_system.h"
#include "Components.h"
#include "events.h"

void DamageSystem::update(Environment* _env)
{
	auto collisions = _env->getEvents<CollisionEvent>();

	for(unsigned i=0;i<collisions.size();i++)
	{
		handleProjectiles(_env, collisions[i]);
	}
}

void DamageSystem::handleProjectiles(Environment* _env, const CollisionEvent& collision)
{
	auto projectile = _env->get<Projectile>();
	auto health = _env->get<Health>();

	unsigned projectile_id = 0;
	unsigned target_id = 0;

	if(_env->hasComponents<Projectile>(collision.entity1))
	{
		projectile_id = collision.entity1;
		target_id = collision.entity2;
	}

	if(_env->hasComponents<Projectile>(collision.entity2))
	{
		projectile_id = collision.entity2;
		target_id = collision.entity1;
	}

	if(projectile_id != 0)
	{
		if(projectile[projectile_id].shooting_entity != target_id)
		{
			if(_env->hasComponents<Solid>(target_id))
			{
				if(_env->hasComponents<Health>(target_id))
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
						
						unsigned targetScoreID = _env->getID(_env->getEntityName(target_id)+"Score"),
								 destroyerScoreID = _env->getID(_env->getEntityName(projectile[projectile_id].shooting_entity)+"Score");

						_env->emit(new DestroyEvent(target_id));
						_env->emit(new ScoreEvent(destroyerScoreID, targetScoreID));
						_env->destroyEntity(target_id);
					}
				}
				
				_env->destroyEntity(projectile_id);
			}
		}
	}
}
