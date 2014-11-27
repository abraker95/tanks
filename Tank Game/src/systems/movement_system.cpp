#define PI 3.14159f
#include <cmath>
#include "components/transform.h"
#include "components/velocity.h"
#include "systems/movement_system.h"

MovementSystem::MovementSystem()
{
}

MovementSystem::~MovementSystem()
{
}

void MovementSystem::update(Environment* env, float dt)
{
	Transform* transform = env->get<Transform>();
	Velocity* velocity= env->get<Velocity>();

	for(unsigned i=0;i<env->maxEntities();i++)
	{
		if(env->hasComponents<Transform, Velocity>(i))
		{
			transform[i].rot += velocity[i].vrot * dt;

			transform[i].x += cosf(transform[i].rot * PI/180.f + PI/2.f) * velocity[i].speed * dt;
			transform[i].y += sinf(transform[i].rot * PI/180.f + PI/2.f) * velocity[i].speed * dt;
		}
	}
}
