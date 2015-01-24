#pragma once
#include <SFML/Graphics.hpp>
#include "core/environment.h"
#include "Managers.h"

class InputSystem : public System
{
	public:
		InputSystem();
		~InputSystem();

		void update(Environment* _env, sf::RenderWindow* win, EntityManager* entity_manager, TextureManager* texture_manager, UI_Manager* _UImgr);
};
