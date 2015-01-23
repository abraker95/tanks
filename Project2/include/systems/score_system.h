#pragma once
#include "core/environment.h"

class ScoreSystem : public System
{
	public:
		ScoreSystem();
		void update(Environment* env);
};

