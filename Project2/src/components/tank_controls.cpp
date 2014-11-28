#include "components\tank_controls.h"

TankControls::TankControls()
{
}

TankControls::TankControls(std::array<sf::Keyboard::Key,5> _keys) : keys(_keys)
{
}

void TankControls::Update()
{
	// update the keystate bitmask
	state.reset();
	for(int j = 0; j<5; j++)
		if(sf::Keyboard::isKeyPressed(keys[j]))
			state.set(j, true);
}