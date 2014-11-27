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
	bool intersectCircleCircle(Transform* transform, BoundingCircle* bounding_circle, unsigned e1, unsigned e2);
	void feedbackCircleCircle(Transform* transform, BoundingCircle* bounding_circle, unsigned e1, unsigned e2);
};
