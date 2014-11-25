#pragma once

#include <SFML/Graphics.hpp>

struct Gun
{
	Gun();

	const float fireCooldown = 0.1f;
	sf::Clock fireClock;
};
