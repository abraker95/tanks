#include <algorithm>
#include <cmath>
#include "math/vector.h"
#include "Components.h"
#include "systems/physics_system.h"
#include "events.h"

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
	auto bounding_box = env->get<BoundingBox>();

	for(unsigned i=0;i<env->maxEntities();i++)
	{
		if(env->hasComponents<Transform, Velocity>(i))
		{
			Vec2f vel = Vec2f::Polar(transform[i].rot + 90.f, velocity[i].speed);

			if(velocity[i].speed != 0.f && env->hasComponents<Solid, BoundingBox>(i))
			{
				while(true)
				{
					float min_toi = 0.f, toi;
					unsigned min_toi_entity;
					Vec2f min_toi_vel;

					for(unsigned j=0;j<env->maxEntities();j++)
					{
						if(env->hasComponents<Transform, Solid, BoundingBox>(j))
						{
							Vec2f vel_corrected = vel;
							toi = avoidCollisionAABB(
								transform[i].pos, bounding_box[i].size, vel_corrected, 
								transform[j].pos, bounding_box[j].size, dt);

							if(toi > 0.f && (min_toi == 0.f || toi < min_toi))
							{
								min_toi = toi;
								min_toi_entity = j;
								min_toi_vel = vel_corrected;
							}
						}
					}

					if(min_toi > 0.f)
					{
						env->emit(new CollisionEvent(i, min_toi_entity));
						vel = min_toi_vel;
					}
					else
						break;
				}
			}


			transform[i].pos += vel * dt;
			transform[i].rot += velocity[i].vrot * dt;
		}
	}
}

// returns true if a collision was avoided
float PhysicsSystem::avoidCollisionAABB(Vec2f& origin1, Vec2f& size1, Vec2f& vel, Vec2f& origin2, Vec2f& size2, float elapsed)
{
	// size of minkowski sum
	Vec2f MSsize = size1 + size2;
	Vec2f normal;

	float t = raycastOnAABB(origin1, vel, origin2, MSsize, normal);
	if(t > 0.f && t < elapsed)
	{
		Vec2f disp = vel * elapsed;
		disp -= normal * normal.dot(disp);
		vel = disp/elapsed;

		return t;
	}
	return 0.f;
}

float PhysicsSystem::raycastOnAABB(Vec2f& point, Vec2f& dir, Vec2f& origin, Vec2f& size, Vec2f& normal)
{
	float minX = origin.x - size.x/2.f, maxX = origin.x + size.x/2.f;
	float minY = origin.y - size.y/2.f, maxY = origin.y + size.y/2.f;
	float t = -1.f;

	float y;
	if(point.x < minX && dir.x != 0.f)
	{
		normal = Vec2f(-1.f, 0.f);
		t = (minX - point.x)/dir.x;
	}

	else if(point.x > maxX && dir.x != 0.f)
	{
		normal = Vec2f(1.f, 0.f);
		t = (maxX - point.x)/dir.x;
	}
	
	y = point.y + t * dir.y;
	if(t >= 0.f && y > minY && y < maxY)
		return t;
	
	float x;
	if(point.y < minY && dir.y != 0.f)
	{
		normal = Vec2f(0.f, -1.f);
		t = (minY - point.y)/dir.y;
	}
	else if(point.y > maxY && dir.y != 0.f)
	{
		normal = Vec2f(0.f, 1.f);
		t = (maxY - point.y)/dir.y;
	}
	
	x = point.x + t * dir.x;
	if(t >= 0.f && x > minX && x < maxX)
		return t;

	return -1.f;
}
