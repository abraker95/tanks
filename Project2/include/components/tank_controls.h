#pragma once
#include <SFML/Window/Keyboard.hpp>
#include <array>
#include <bitset>
#include "core/environment.h"

struct TankControls : public Component<TankControls>
{
	TankControls(std::array<sf::Keyboard::Key, 5> _keys);

	enum
	{
		TURN_RIGHT = 0,
		TURN_LEFT, 
		GO_FORWARD,
		GO_BACKWARD,
		FIRE,
	};

	std::array<sf::Keyboard::Key, 5> keys;
	std::bitset<5> state;
};
