#pragma once
#include <SFML/Graphics.hpp>
#include "core/environment.h"
#include "managers/managers.h"

class InputSystem : public System
{
	public:
		InputSystem();
		~InputSystem();

		void update(Environment* _env, sf::RenderWindow* win, Managers* managers);
};
