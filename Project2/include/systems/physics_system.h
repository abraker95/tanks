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
	// name convention: the shape with less points comes first
	void handleCircleCircleCollisions(Environment* env, unsigned circle1, unsigned circle2, unsigned obstacle);
	bool intersectCircleCircle(const Vec2f& c1, float r1, const Vec2f& c2, float r2);
	Vec2f feedbackCircleCircle(const Vec2f& c1, float r1, const Vec2f& c2, float r2);

	void handleRectCircleCollisions(Environment* env, unsigned box, unsigned circle, unsigned obstacle);
	bool intersectRectCircle(const Vec2f& p1, const Vec2f& s, const Vec2f& c2, float r2);
	Vec2f feedbackRectCircle(Vec2f& p1, const Vec2f& s, Vec2f& c2, float r2);

	bool intersectLineCircle(const Vec2f& s1, const Vec2f& f1, const Vec2f& c1, float r2);
	bool intersectPointCircle(const Vec2f& p1, const Vec2f& c1, float r2);
	bool intersectPointRect(const Vec2f& p1, const Vec2f& p2, const Vec2f& s);
	bool intersectRectRect(const Vec2f& p1, const Vec2f& s1, const Vec2f& p2, const Vec2f& s2);
};
