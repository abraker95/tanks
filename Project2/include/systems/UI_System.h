#pragma once
#include <functional>
#include "core/environment.h"
#include "managers/managers.h"

class UISystem : public System
{
	public:
		UISystem();
		~UISystem();

		void update(Environment* _mainEnv, Managers* managers);
};
