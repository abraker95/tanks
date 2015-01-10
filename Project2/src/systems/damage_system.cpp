#include <iostream>
#include "systems/damage_system.h"
#include "Components.h"
#include "events.h"

void DamageSystem::update(Environment* _gameEnv)
{
	auto collisions = _gameEnv->getEvents<CollisionEvent>();

	for(unsigned i=0;i<collisions.size();i++)
	{
		handleProjectiles(_gameEnv, collisions[i]);
	}
}

void DamageSystem::handleProjectiles(Environment* _gameEnv, const CollisionEvent& collision)
{
	auto projectile = _gameEnv->get<Projectile>();
	auto health = _gameEnv->get<Health>();

	unsigned projectile_id = 0;
	unsigned target_id = 0;

	if(_gameEnv->hasComponents<Projectile>(collision.entity1))
	{
		projectile_id = collision.entity1;
		target_id = collision.entity2;
	}

	if(_gameEnv->hasComponents<Projectile>(collision.entity2))
	{
		projectile_id = collision.entity2;
		target_id = collision.entity1;
	}

	if(projectile_id != 0)
	{
		if(projectile[projectile_id].shooting_entity != target_id)
		{
			if(_gameEnv->hasComponents<Solid>(target_id))
			{
				if(_gameEnv->hasComponents<Health>(target_id))
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
						
						_gameEnv->emit(new DestroyEvent(target_id));
						_gameEnv->destroyEntity(target_id);
					}
				}
				
				_gameEnv->destroyEntity(projectile_id);
			}
		}
	}
}
