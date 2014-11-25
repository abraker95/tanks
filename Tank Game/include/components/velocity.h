#pragma once

struct Velocity
{
	Velocity();
	Velocity(float vx, float vy, float vrot);

	float vx, vy;
	float vrot;
};
