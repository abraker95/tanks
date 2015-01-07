#pragma once
#include <SFML/Graphics.hpp>
#include "core/environment.h"
#include "Managers.h"

class InputSystem
{
	public:
		InputSystem();
		~InputSystem();

		void update(Environment* _env, EntityManager* entity_manager, TextureManager* texture_manager, CPUManager* _cpuMgr, UI_Manager* _uiMgr);
};
