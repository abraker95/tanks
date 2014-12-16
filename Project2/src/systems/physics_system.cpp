#include <algorithm>
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

	for(unsigned i=0;i<env->maxEntities();i++)
	{
		if(env->hasComponents<Transform, Velocity>(i))
		{
			transform[i].rot += velocity[i].vrot * dt;
			transform[i].pos += Vec2f(Vec2f::Polar(transform[i].rot + 90.f, velocity[i].speed * dt));
		}

		for(unsigned j=0;j<env->maxEntities();j++)
		{
			handleCircleCircleCollisions(env, i, j);			
			handleRectCircleCollisions(env, i, j);
		}
	}
}

void PhysicsSystem::handleCircleCircleCollisions(Environment* env, unsigned i, unsigned j)
{
	auto transform = env->get<Transform>();
	auto bounding_circle = env->get<BoundingCircle>();

	if(i != j && 
		env->hasComponents<Transform, BoundingCircle>(i) && 
		env->hasComponents<Transform, BoundingCircle>(j))
	{
		if(intersectCircleCircle(
			transform[i].pos, bounding_circle[i].radius,
			transform[j].pos, bounding_circle[j].radius))

		{
			env->emit(new CollisionEvent(i, j));

			if(	env->hasComponents<Solid>(i) &&
				env->hasComponents<Solid>(j))
			{
				transform[i].pos += feedbackCircleCircle(
					transform[i].pos, bounding_circle[i].radius,
					transform[j].pos, bounding_circle[j].radius);
			}
		}
	}
}

bool PhysicsSystem::intersectCircleCircle(
	const Vec2f& c1, float r1, const Vec2f& c2, float r2)
{
	Vec2f delta = c1 - c2;
	float minDist = r1 + r2;

	return (delta.lengthSquared() < minDist * minDist);
}

Vec2f PhysicsSystem::feedbackCircleCircle(
	const Vec2f& c1, float r1, const Vec2f& c2, float r2)
{
	Vec2f delta = c1 - c2;

	delta.normalize();
	delta *= r1 + r2;

	return c2 + delta - c1;
}

void PhysicsSystem::handleRectCircleCollisions(Environment* env, unsigned i, unsigned j)
{
	if(i == j) 
		return;

	unsigned box = 0;
	unsigned circle = 0;

	auto transform = env->get<Transform>();
	auto bounding_circle = env->get<BoundingCircle>();
	auto bounding_box = env->get<BoundingBox>();

	if(
		env->hasComponents<Transform, BoundingCircle>(i) && 
		env->hasComponents<Transform, BoundingBox>(j))
	{
		circle = i;
		box = j;
	}

	else if(
		env->hasComponents<Transform, BoundingCircle>(j) && 
		env->hasComponents<Transform, BoundingBox>(i))
	{
		circle = j;
		box = i;
	}

	else
		return;
	
	if(intersectRectCircle(
		transform[box].pos, bounding_box[box].size, 
		transform[circle].pos, bounding_circle[circle].radius))
	{
		env->emit(new CollisionEvent(i, j));
		if(	env->hasComponents<Solid>(i) &&
			env->hasComponents<Solid>(j))
		{
			Vec2f delta = feedbackRectCircle(
				transform[box].pos, bounding_box[box].size,
				transform[circle].pos, bounding_circle[circle].radius);

			if(i == circle)
				transform[i].pos += delta;
			else
				transform[j].pos -= delta;
		}
	}
}

bool PhysicsSystem::intersectRectCircle(const Vec2f& p1, const Vec2f& s1, const Vec2f& c2, float r2)
{
	if(intersectPointRect(c2, p1, s1))
		return true;
	
	// the four corners
	Vec2f cr1(p1.x - s1.x/2, p1.y - s1.y/2);
	Vec2f cr2(p1.x + s1.x/2, p1.y - s1.y/2);
	Vec2f cr3(p1.x + s1.x/2, p1.y + s1.y/2);
	Vec2f cr4(p1.x - s1.x/2, p1.y + s1.y/2);

	return (
		intersectLineCircle(cr1, cr2, c2, r2) ||
		intersectLineCircle(cr2, cr3, c2, r2) ||
		intersectLineCircle(cr3, cr4, c2, r2) ||
		intersectLineCircle(cr4, cr1, c2, r2)
	);
}

Vec2f PhysicsSystem::feedbackRectCircle(Vec2f& p1, const Vec2f& s, Vec2f& c2, float r2)
{
	Vec2f delta;

	float dx1 = (c2.x + r2) - (p1.x - s.x/2.f);
	float dx2 = (p1.x + s.x/2.f) - (c2.x - r2);

	delta.x = std::min(dx1, dx2);

	float dy1 = (c2.y + r2) - (p1.y - s.y/2.f);
	float dy2 = (p1.y + s.y/2.f) - (c2.y - r2);

	delta.y = std::min(dy1, dy2);
	
	if(delta.x < delta.y) delta.y = 0.f;
	else delta.x = 0.f;
	
	return delta;
}

bool PhysicsSystem::intersectLineCircle(const Vec2f& s1, const Vec2f& f1, const Vec2f& c2, float r2)
{
	Vec2f lineVec = f1 - s1;
	Vec2f lineCenter = c2 - s1;
	
	float length = lineVec.length();
	Vec2f unitLineVec = lineVec / length;
	
	float projDist = lineCenter.dot(unitLineVec);
	Vec2f proj = unitLineVec * projDist + s1;

	return (projDist >= 0.f && projDist <= length && intersectPointCircle(proj, c2, r2));
}

bool PhysicsSystem::intersectPointCircle(const Vec2f& p1, const Vec2f& c2, float r2)
{
	Vec2f delta = p1 - c2;
	return (delta.lengthSquared() <= r2 * r2);
}

bool PhysicsSystem::intersectPointRect(const Vec2f& p1, const Vec2f& p2, const Vec2f& s)
{
	return (
		p1.x >= p2.x - s.x/2.f && p1.x <= p2.x + s.x/2.f &&
		p1.y >= p2.y - s.y/2.f && p1.y <= p2.y + s.y/2.f
	);
}
