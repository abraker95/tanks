#pragma once

struct Transform
{
	Transform();
	Transform(float x, float y, float rot);

	float x, y;
	float rot;
};
