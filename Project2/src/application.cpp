#include "application.h"
#include <array>
#include "Components.h"
#include "events.h"
#include "utils.h"

Application::Application(): mainEnv(128)
{
// [CORE DECLARATIONS]
	window = new sf::RenderWindow(sf::VideoMode(1024, 720), "https://github.com/Sherushe/tanks.git (pre-alpha branch)");
	fullscreen = false;
	//window->setFramerateLimit(60);
	
	// [ENTITY CREATION]
	managers.map_loader.createMap(&mainEnv, &managers.texture_manager, "maps/dev1.map");
	managers.entity_manager.NewGame(&mainEnv, &managers.texture_manager, &managers.score_manager);
	managers.UI_manager.CreateMenu(&mainEnv, window, fullscreen); // needs to go after entity manager

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
	render_system = new RenderSystem(window, &managers.font_manager);
	expiring_system = new ExpiringSystem();
	physics_system = new PhysicsSystem();
	view_system = new ViewSystem();
	damage_system = new DamageSystem();
	hud_system = new HUDSystem(&mainEnv);
	score_system = new ScoreSystem();
	net_system = new NetworkSystem();
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
		elapsed = clock.restart();
		update(elapsed.asSeconds());
	}

	return EXIT_SUCCESS;
}

void Application::update(float dt)
{
	mainEnv.updateWrapper(net_system);

	const auto& monitoring_results = mainEnv.resetMonitoring(dt);
	mainEnv.updateWrapper(input_system, window, &managers);
	mainEnv.updateWrapper(ui_system, &managers);
	mainEnv.updateWrapper(expiring_system, dt);
	mainEnv.updateWrapper(physics_system, dt);
	mainEnv.updateWrapper(damage_system, &managers);
	mainEnv.updateWrapper(score_system);
	mainEnv.updateWrapper(view_system, window, dt);
	mainEnv.updateWrapper(render_system, hud_system, &managers, window, monitoring_results);
}
