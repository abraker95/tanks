#pragma once
#include <functional>
#include "core/environment.h"

class UISystem
{
	public:
		UISystem();
		UISystem(Environment* _env);
		~UISystem();

		void update(Environment* _env, UI_Manager* _uiMgr, EntityManager* _entMgr, TextureManager* _texMgr);
};
