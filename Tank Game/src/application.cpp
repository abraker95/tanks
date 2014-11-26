#include "application.h"
#include "components/transform.h"
#include "components/texture_handle.h"
#include "systems/sprite_manager.h"

Application::Application()
{
	window = new sf::RenderWindow(sf::VideoMode(1024, 720), "[game name] ( dev version )");

	main_env = new Environment(10);
	main_env->alloc<Transform, TextureHandle>();
}

Application::~Application()
{
	if(main_env)
		delete main_env;

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
