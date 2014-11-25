#pragma once
#include <SFML/Window/Keyboard.hpp>

struct TankControls
{
	TankControls();

	TankControls(
		sf::Keyboard::Key turn_right,
		sf::Keyboard::Key turn_left,
		sf::Keyboard::Key go_forward,
		sf::Keyboard::Key go_backward,
		sf::Keyboard::Key fire_bullet);

	enum
	{
		TURN_RIGHT,
		TURN_LEFT, 
		GO_FORWARD,
		GO_BACKWARD,
		FIRE_BULLET,
	};

	sf::Keyboard::Key keys[5];
	bool state[5];
};
