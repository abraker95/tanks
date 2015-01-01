#pragma once
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <array>
#include <bitset>

class InputControls_Mgr
{
	public:
		InputControls_Mgr();
		virtual ~InputControls_Mgr();

		/// \TODO: Doesn't SFML have this?
		enum MOUSE
		{
			LEFT = 0,
			MIDDLE,
			RIGHT
		};

		std::array<sf::Mouse::Button, 3> buttons;
		std::bitset<3> mousePressState, mouseClickState;

		int numKeys;
		std::bitset<sf::Keyboard::KeyCount> keyPressState, keyClickState;
};

extern InputControls_Mgr input_manager;