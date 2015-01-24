#pragma once
#include "core/environment.h"

class NetworkSystem: public System
{
public:
	NetworkSystem();
	~NetworkSystem();

	void update(Environment* _mainEnv);
};

