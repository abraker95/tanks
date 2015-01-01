#include "application.h"
#include <array>
#include "Components.h"
#include "events.h"
#include "utils.h"

Application::Application() : main_env(128)
{
// [CORE DECLARATIONS]

	window = new sf::RenderWindow(sf::VideoMode(1024, 720), "https://github.com/Sherushe/tanks.git (pre-alpha branch)");

// [ENTITY CREATION]
	map_loader.createMap(&main_env, &texture_manager, "maps/dev1.map");
	entity_manager.NewGame(&main_env, &texture_manager);

	UI_manager.CreateMenu(&main_env, window);
	cpu_manager.createCPUMgr(&main_env, window);

	/*main_env.createEntity(
		new StdComponent<int>(new int(2)),
		new StdComponent<int>(new int(4))
		);*/

	// Testing Radio Button
	/*main_env.createEntity(
		Transform(500.f, 30.f, 0.f),
		MouseControls(),
		GUIObj(GUIObj::RADIO, []()->void* { ; return nullptr; }),
		TextureHandle("Button.png")
		);*/

// [FACTORY CONSTRUCTS]
	ui_system = new UISystem(&main_env);
	input_system = new InputSystem();
	render_system = new RenderSystem(window);
	expiring_system = new ExpiringSystem();
	physics_system = new PhysicsSystem();
	view_system = new ViewSystem();
	damage_system = new DamageSystem();
}

Application::~Application()
{
	delete ui_system;
	delete input_system;
	delete render_system;
	delete expiring_system;
	delete physics_system;
	delete view_system;
	delete damage_system;

	if(window)	delete window;
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

		sf::Time elapsed = cpu_manager.update();
		update(elapsed.asSeconds());
	}

	return EXIT_SUCCESS;
}

void Application::update(float dt)
{
	main_env.updateWrapper(input_system, &entity_manager, &texture_manager);
	main_env.updateWrapper(ui_system, &entity_manager, &texture_manager);
	main_env.updateWrapper(expiring_system, dt);
	main_env.updateWrapper(physics_system, dt);
	main_env.updateWrapper(damage_system);
	main_env.updateWrapper(view_system, window, dt);
	main_env.updateWrapper(render_system, window);
}
