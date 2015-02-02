#include "application.h"
#include <array>
#include <ctime>
#include "Components.h"
#include "events.h"
#include "utils.h"

Application::Application(): mainEnv(128)
{
	// init prng
	srand((unsigned int)time(NULL));

// [CORE DECLARATIONS]
	window = new sf::RenderWindow(sf::VideoMode(1024, 720), "https://github.com/Sherushe/tanks.git (pre-alpha branch)");
	fullscreen = false;
	//window->setFramerateLimit(60);
	
// [ENTITY CREATION]
	managers.UI_manager.CreateMenu(&mainEnv, window, &managers, fullscreen); // needs to go after entity manager

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
	mainEnv.updateWrapper(net_system, &managers);

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
