#pragma once

#include <SFML/Graphics.hpp>
#include "core/environment.h"

struct Gun : public Component<Gun>
{
	Gun();

	float fireCooldown = 0.5f;
	sf::Clock fireClock;
};
