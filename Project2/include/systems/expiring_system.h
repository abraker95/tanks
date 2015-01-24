#pragma once
#include "core/environment.h"
#include "components/expires.h"

class ExpiringSystem : public System
{
public:
	ExpiringSystem();
	~ExpiringSystem();

	void update(Environment* env, float dt);
};
