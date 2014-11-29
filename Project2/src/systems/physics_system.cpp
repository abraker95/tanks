#define PI 3.14159f
#include <cmath>
#include "Components.h"
#include "systems/physics_system.h"

PhysicsSystem::PhysicsSystem()
{

}

PhysicsSystem::~PhysicsSystem()
{
}

void PhysicsSystem::update(Environment* env, float dt)
{
	Velocity* velocity = env->get<Velocity>();
	Transform* transform = env->get<Transform>();
	BoundingCircle* bounding_circle = env->get<BoundingCircle>();

	for(unsigned i=0;i<env->maxEntities();i++)
	{
		if(env->hasComponents<Transform, Velocity>(i))
		{
			transform[i].rot += velocity[i].vrot * dt;

			transform[i].x += cosf(transform[i].rot * PI/180.f + PI/2.f) * velocity[i].speed * dt;
			transform[i].y += sinf(transform[i].rot * PI/180.f + PI/2.f) * velocity[i].speed * dt;
		}

		for(unsigned j=0;j<env->maxEntities();j++)
		{
			if(i != j && 
				env->hasComponents<Transform, BoundingCircle>(i) && 
				env->hasComponents<Transform, BoundingCircle>(j))
			{
				if(intersectCircleCircle(transform, bounding_circle, i, j))
				{
					bounding_circle[i].collision = true;
					bounding_circle[j].collision = true;

					if(	!env->hasComponents<Projectile>(i) &&
						!env->hasComponents<Projectile>(j))
					{
						feedbackCircleCircle(transform, bounding_circle, i, j);
					}
				}
				else
				{
					bounding_circle[i].collision = false;
					bounding_circle[j].collision = false;
				}	
			}
		}
	}
}

bool PhysicsSystem::intersectCircleCircle(
	Transform* transform, BoundingCircle* bounding_circle, 
	unsigned e1, unsigned e2)
{
	float dx = transform[e1].x - transform[e2].x;
	float dy = transform[e1].y - transform[e2].y;

	float min_dist = bounding_circle[e1].radius + bounding_circle[e2].radius;

	return (dx * dx + dy * dy < min_dist * min_dist);
}

void PhysicsSystem::feedbackCircleCircle(
	Transform* transform, BoundingCircle* bounding_circle, 
	unsigned e1, unsigned e2)
{
	float dx = transform[e1].x - transform[e2].x;
	float dy = transform[e1].y - transform[e2].y;
	float length = sqrtf(dx * dx + dy * dy);
	float min_dist = bounding_circle[e1].radius + bounding_circle[e2].radius;

	dx /= length;
	dy /= length;

	dx *= min_dist;
	dy *= min_dist;

	transform[e1].x = transform[e2].x + dx;
	transform[e1].y = transform[e2].y + dy;
}
