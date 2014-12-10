#pragma once
#include "core/environment.h"

class MapCreationSystem
{
public:
	void update(Environment* env);
private:
	void buildVA(Environment* env, int id);
};
