#include "managers/InputControls_Mgr.h"

InputControls_Mgr::InputControls_Mgr()
{
	buttons[0] = sf::Mouse::Button::Left;
	buttons[1] = sf::Mouse::Button::Middle;
	buttons[2] = sf::Mouse::Button::Right;
}


InputControls_Mgr::~InputControls_Mgr()
{}

InputControls_Mgr input_manager;
