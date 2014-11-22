#pragma once

struct Transform
{
	Transform(float x, float y, float rot) : x(x), y(y), rot(rot) {}

	float x, y;
	float rot;
};
