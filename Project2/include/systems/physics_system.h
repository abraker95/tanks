#pragma once
#include "core/environment.h"
#include "math/vector.h"

class PhysicsSystem
{
public:
	PhysicsSystem();
	~PhysicsSystem();

	void update(Environment* _gameEnv, float dt);

private:
	float avoidCollisionAABB(Vec2f& origin1, Vec2f& size1, Vec2f& vel, Vec2f& origin2, Vec2f& size2, float elapsed);
	float raycastOnAABB(Vec2f& point, Vec2f& dir, Vec2f& origin, Vec2f& size, Vec2f& normal);
};
