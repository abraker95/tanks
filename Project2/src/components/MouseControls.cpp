#include "components\MouseControls.h"

MouseControls::MouseControls() {}
MouseControls::MouseControls(std::array<sf::Mouse::Button, 3> _buttons): buttons(_buttons) {}

void MouseControls::Update()
{
	// update the keystate bitmask
	state.reset();
	for(int j = 0; j<3; j++)
		if(sf::Mouse::isButtonPressed(buttons[j]))
			state.set(j, true);
}