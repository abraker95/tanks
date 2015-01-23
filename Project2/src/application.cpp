#include "application.h"
#include <array>
#include "Components.h"
#include "events.h"
#include "utils.h"

Application::Application(): mainEnv(128)
{
// [CORE DECLARATIONS]
	window = new sf::RenderWindow(sf::VideoMode(1024, 720), "https://github.com/Sherushe/tanks.git (pre-alpha branch)");
	//window->setFramerateLimit(60);
	
	// [ENTITY CREATION]
	map_loader.createMap(&mainEnv, &texture_manager, "maps/dev1.map");
	entity_manager.NewGame(&mainEnv, &texture_manager);

	UI_manager.CreateMenu(&mainEnv, window); // needs to go after entity manager

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
	ui_system = new UISystem();
	input_system = new InputSystem();
	render_system = new RenderSystem(window, &font_manager);
	expiring_system = new ExpiringSystem();
	physics_system = new PhysicsSystem();
	view_system = new ViewSystem();
	damage_system = new DamageSystem();
	hud_system = new HUDSystem(&mainEnv);
	score_system = new ScoreSystem();
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
	sf::Clock clock;
	sf::Time elapsed;

	while(window->isOpen())
	{
		sf::Event event;
		while(window->pollEvent(event))
		{
			if(event.type == sf::Event::Closed)
				window->close();
		}

		elapsed = clock.restart();
		update(elapsed.asSeconds());
	}

	return EXIT_SUCCESS;
}

void Application::update(float dt)
{
	const auto& monitoring_results = mainEnv.resetMonitoring(dt);

	mainEnv.updateWrapper(input_system, &entity_manager, &texture_manager, &UI_manager);
	mainEnv.updateWrapper(ui_system, &UI_manager, &entity_manager, &texture_manager);
	mainEnv.updateWrapper(expiring_system, dt);
	mainEnv.updateWrapper(physics_system, dt);
	mainEnv.updateWrapper(damage_system);
	mainEnv.updateWrapper(score_system);
	mainEnv.updateWrapper(view_system, window, dt);
	mainEnv.updateWrapper(render_system, hud_system, window, monitoring_results);
}
