#include "application.h"
#include "components/transform.h"
#include "components/velocity.h"
#include "components/tank_controls.h"
#include "components/texture_handle.h"

Application::Application() : main_env(32)
{
	window = new sf::RenderWindow(sf::VideoMode(1024, 720), "https://github.com/Sherushe/tanks.git (experimental branch)");

	// TODO: automatically manage this
	main_env.alloc<Transform, Velocity, TextureHandle, RenderProperties, TankControls>();

	// double-braces init because of std::array
	std::array<sf::Keyboard::Key, 5> p1_keys = {{ sf::Keyboard::Right, sf::Keyboard::Left, sf::Keyboard::Up, sf::Keyboard::Down, sf::Keyboard::Space }};

	main_env.createEntity(
		Transform(500.f, 300.f, 0.f),
		Velocity(0.f, 0.f),
		TextureHandle("Tank_0.png"),
		TankControls(p1_keys)
	);
}

Application::~Application()
{
	// TODO: automatically manage this
	main_env.dealloc<Transform, Velocity, TextureHandle, RenderProperties, TankControls>();

	if(window)
		delete window;
}

int Application::run()
{
	sf::Clock clock;
	while(window->isOpen())
	{
		sf::Event event;
		while(window->pollEvent(event))
		{
			if(event.type == sf::Event::Closed)
				window->close();
		}

		sf::Time elapsed = clock.restart();
		update(elapsed.asSeconds());
	}

	return EXIT_SUCCESS;
}

void Application::update(float dt)
{
	input_system.update(&main_env);
	movement_system.update(&main_env, dt);
	texture_manager.update(&main_env);
	render_system.update(&main_env, window);
}
