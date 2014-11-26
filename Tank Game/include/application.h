#pragma once
#include <SFML/Graphics.hpp>
#include "core/environment.h"

class Application
{
public:
	Application();
	~Application();

	int run();	

private:
	sf::RenderWindow* window;

	Environment* main_env;
};
