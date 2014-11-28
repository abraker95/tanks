#pragma once
#include "core/environment.h"

class MovementSystem
{
public:
	MovementSystem();
	~MovementSystem();

	void update(Environment* env, float dt);
};
