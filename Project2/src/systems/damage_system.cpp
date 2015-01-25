#include <iostream>
#include "systems/damage_system.h"
#include "Components.h"
#include "events.h"

void DamageSystem::update(Environment* _env, Managers* managers)
{
	auto collisions = _env->getEvents<CollisionEvent>();

	for(unsigned i=0;i<collisions.size();i++)
	{
		handleProjectiles(_env, collisions[i], managers);
	}
}

void DamageSystem::handleProjectiles(Environment* _env, const CollisionEvent& collision, Managers* managers)
{
	auto projectile = _env->get<Projectile>();
	auto health = _env->get<Health>();
	auto player = _env->get<Player>();
	auto transform = _env->get<Transform>();

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
					int shooting_entity = projectile[projectile_id].shooting_entity;

					health[target_id].addHealth(-damage);

					if(health[target_id].hasHealth())
					{
						std::cout<<" *hits*"<<std::endl;
					}

					else
					{
						std::cout<<" *BOOM*"<<std::endl;

						// if explosible, spawn explosion
						if(_env->hasComponents<Explosible, Transform>(target_id))
						{
							managers->entity_manager.spawnExplosion(_env, &managers->texture_manager, transform[target_id].pos);
						}
						
						_env->emit(new DestroyEvent(target_id));


						if(_env->hasComponents<Player>(target_id))
						{
							// if it's a player, only make it disappear
							// the tank is destroyed but not the player itself
							managers->entity_manager.killPlayer(_env, target_id);
							managers->score_manager.addLoss(player[target_id].player_id);

							// if only 1 player left
							if(managers->entity_manager.getNumLivingTanks() == 1)
							{
								managers->score_manager.addWin(player[shooting_entity].player_id);
								_env->emit(new GameOverEvent(shooting_entity));
							}
						}

						else
						{
							_env->destroyEntity(target_id);
						}

					}
				}
				
				_env->destroyEntity(projectile_id);
			}
		}
	}
}
