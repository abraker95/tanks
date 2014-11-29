#pragma once
#include <SFML/Window/Mouse.hpp>
#include <array>
#include <bitset>
#include "core/environment.h"

class MouseControls: public Component<MouseControls>
{
public:
	MouseControls();
	//MouseControls(std::array<sf::Mouse::Button, 3> _buttons);

	enum
	{
		LEFT = 0,
		MIDDLE,
		RIGHT
	};

	std::array<sf::Mouse::Button, 3> buttons;
	std::array<int, 2> pos;
	std::bitset<3> pressState, clickState;
};
