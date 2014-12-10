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
	auto velocity = env->get<Velocity>();
	auto transform = env->get<Transform>();
	auto bounding_circle = env->get<BoundingCircle>();

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
				if(intersectCircleCircle(
					transform[i].x, transform[i].y, bounding_circle[i].radius,
					transform[j].x, transform[j].y, bounding_circle[j].radius))

				{
					bounding_circle[i].collision = true;
					bounding_circle[j].collision = true;

					if(	!env->hasComponents<Projectile>(i) &&
						!env->hasComponents<Projectile>(j))
					{
						feedbackCircleCircle(
							transform[i].x, transform[i].y, bounding_circle[i].radius,
							transform[j].x, transform[j].y, bounding_circle[j].radius);
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
	float x1, float y1, float r1, float x2, float y2, float r2)
{
	float dx = x1 - x2;
	float dy = y1 - y2;
	float min_dist = r1 + r2;

	return (dx * dx + dy * dy < min_dist * min_dist);
}

void PhysicsSystem::feedbackCircleCircle(
	float& x1, float& y1, float r1, float x2, float y2, float r2)
{
	float dx = x1 - x2;
	float dy = y1 - y2;
	float length = sqrtf(dx * dx + dy * dy);
	float min_dist = r1 + r2;

	dx /= length;
	dy /= length;

	dx *= min_dist;
	dy *= min_dist;

	x1 = x2 + dx;
	y1 = y2 + dy;
}
