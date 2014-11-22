#pragma once
#include <SFML/Graphics.hpp>

class Application
{
public:
	Application();
	~Application();

	int run();	

private:
	sf::RenderWindow* window;
};
