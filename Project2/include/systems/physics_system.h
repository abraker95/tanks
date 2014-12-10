#pragma once
#include "core/environment.h"
#include "components/transform.h"
#include "components/bounding_circle.h"

class PhysicsSystem
{
public:
	PhysicsSystem();
	~PhysicsSystem();

	void update(Environment* env, float dt);

private:
	bool intersectCircleCircle(float x1, float y1, float r1, float x2, float y2, float r2);
	void feedbackCircleCircle(float& x1, float& y1, float r1, float x2, float y2, float r2);
};
