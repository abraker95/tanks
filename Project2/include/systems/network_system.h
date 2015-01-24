#pragma once
#include "core/environment.h"

class NetworkSystem
{
public:
	NetworkSystem();
	~NetworkSystem();

	void update(Environment* _mainEnv);
};

