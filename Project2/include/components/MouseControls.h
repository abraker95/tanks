#pragma once
#include <SFML/Window/Mouse.hpp>
#include <array>
#include <bitset>
#include "core/environment.h"

/// \TODO: This can be a singleton. There is no reason mouse states should be copied as entities.
class MouseControls: public Component<MouseControls>
{
public:
	MouseControls();

	enum
	{
		LEFT = 0,
		MIDDLE,
		RIGHT
	};

	std::array<sf::Mouse::Button, 3> buttons;
	std::bitset<3> pressState, clickState;
};
