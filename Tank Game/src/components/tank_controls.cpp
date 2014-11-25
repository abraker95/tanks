#include "components\tank_controls.h"

TankControls::TankControls()
{
}

TankControls::TankControls(
	sf::Keyboard::Key turn_right,
	sf::Keyboard::Key turn_left,
	sf::Keyboard::Key go_forward,
	sf::Keyboard::Key go_backward,
	sf::Keyboard::Key fire_bullet) 
:
	keys{turn_right, turn_left, go_forward, go_backward, fire_bullet},
	state{false, false, false, false, false} 
{
}
