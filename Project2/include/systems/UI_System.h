#pragma once
#include <functional>
#include "core/environment.h"

class UISystem
{
	public:
		UISystem();
		~UISystem();

		void update(Environment* _mainEnv, UI_Manager* _uiMgr, EntityManager* _entMgr, TextureManager* _texMgr);
};
