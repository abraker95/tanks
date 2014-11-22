#include "application.h"

Application::Application()
{
	window = new sf::RenderWindow(sf::VideoMode(1024, 720), "[game name] ( dev version )");
}

Application::~Application()
{
	if(window)
		delete window;
}

int Application::run()
{
	while(window->isOpen())
	{
		sf::Event event;
		while(window->pollEvent(event))
		{
			if(event.type == sf::Event::Closed)
				window->close();
		}
		// run systems here
	}

	return EXIT_SUCCESS;
}
