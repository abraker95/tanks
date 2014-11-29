#include "application.h"
#include <array>
#include "Components.h"

Application::Application() : main_env(128)
{
	window = new sf::RenderWindow(sf::VideoMode(1024, 720), "https://github.com/Sherushe/tanks.git (pre-alpha branch)");

	// TODO: automatically manage this
	main_env.alloc<
		Transform, Velocity, TextureHandle, RenderProperties, TankControls, 
		Expires, BoundingCircle, MouseControls, Projectile, Gun, UserInterface>();

	// double-braces init because of std::array
	std::array<sf::Keyboard::Key, 5> p1_keys = {{ sf::Keyboard::Right, sf::Keyboard::Left, sf::Keyboard::Up, sf::Keyboard::Down, sf::Keyboard::Space }};

	main_env.createEntity(
		Transform(300.f, 300.f, 0.f),
		Velocity(0.f, 0.f),
		TextureHandle("Tank_0.png"),
		TankControls(p1_keys),
		BoundingCircle(),
		Gun()
	);

	std::array<sf::Keyboard::Key, 5> p2_keys = {{ sf::Keyboard::D, sf::Keyboard::A, sf::Keyboard::W, sf::Keyboard::S, sf::Keyboard::F }};

	main_env.createEntity(
		Transform(800.f, 300.f, 0.f),
		Velocity(0.f, 0.f),
		TextureHandle("Tank_0.png"),
		TankControls(p2_keys),
		BoundingCircle(),
		Gun()
	);

	// testing mouse features
	main_env.createEntity(
		Transform(200.f, 30.f, 0.f),
		MouseControls(),
		UserInterface(),
		TextureHandle("Button.png")
	);
}

Application::~Application()
{
	// TODO: automatically manage this
	main_env.dealloc<
		Transform, Velocity, TextureHandle, RenderProperties, TankControls, 
		Expires, BoundingCircle, MouseControls, Projectile, Gun, UserInterface>();

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
	expiring_system.update(&main_env, dt);

	physics_system.update(&main_env, dt);

	texture_manager.update(&main_env);
	render_system.update(&main_env, window);
}
