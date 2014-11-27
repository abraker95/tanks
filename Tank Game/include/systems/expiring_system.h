#pragma once
#include "core/environment.h"
#include "components/expires.h"

class ExpiringSystem
{
public:
	ExpiringSystem();
	~ExpiringSystem();

	void update(Environment* env, float dt);
};
