#pragma once

#include <SFML/Graphics.hpp>
#include "core/environment.h"

struct Gun : public Component<Gun>
{
	Gun();

	const float fireCooldown = 0.1f;
	sf::Clock fireClock;
};
