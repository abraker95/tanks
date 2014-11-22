#pragma once

struct Velocity
{
	Velocity(float vx, float vy, float vrot) : vx(vx), vy(vy), vrot(vrot) {}

	float vx, vy;
	float vrot;
};
