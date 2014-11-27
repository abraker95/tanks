#pragma once

#include <SFML/Graphics.hpp>
#include "core/environment.h"

class InputSystem
{
public:
	InputSystem();
	~InputSystem();

	void update(Environment* env);
};
