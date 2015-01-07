#pragma once
#include <functional>
#include "core/environment.h"

class UISystem
{
	public:
		UISystem();
		~UISystem();

		void update(Environment* _env, Environment* _uiEnv, UI_Manager* _uiMgr, EntityManager* _entMgr, TextureManager* _texMgr);
};
