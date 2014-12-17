#pragma once
#include <SFML/Window/Keyboard.hpp>
#include <array>
#include <bitset>
#include "core/environment.h"

/// \TODO: This can be a singleton. There is no reason mouse states should be copied as entities.
class KeyControls: public Component<KeyControls>
{
	public:
		KeyControls();

		int numKeys;
		std::bitset<256> pressState, clickState;

};

