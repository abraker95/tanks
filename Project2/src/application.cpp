#include "application.h"
#include <array>
#include "Components.h"

Application::Application() : main_env(64)
{
// [CORE DECLARATIONS]

	window = new sf::RenderWindow(sf::VideoMode(1024, 720), "https://github.com/Sherushe/tanks.git (pre-alpha branch)");

// [ENTITY CREATION]
	// tilemap
	int* map = new int[20*12]{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 2, 1, 1, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 1, 1, 1, 1, 0, 0, 4, 1, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 1, 1, 1, 5, 0, 0, 0, 4, 1, 1, 1, 3, 0, 2, 1, 1, 1, 1,
		0, 0, 1, 1, 5, 0, 0, 0, 0, 0, 0, 4, 1, 1, 1, 1, 1, 1, 1, 1,
		0, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 4, 1, 1, 1, 5, 0, 0, 0,
		2, 1, 1, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		1, 1, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		1, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	};

	main_env.createEntity(
		new MapDesc(map, 20, 12, sf::Vector2u(64, 64)),
		new TextureHandle("Tilesheet.png")
	);

	// double-braces init because of std::array
	std::array<sf::Keyboard::Key, 5> p1_keys = {{ sf::Keyboard::Right, sf::Keyboard::Left, sf::Keyboard::Up, sf::Keyboard::Down, sf::Keyboard::Space }};


	unsigned tank1 = main_env.createEntity(
		new Transform(200.f, 300.f, 0.f),
		new Velocity(0.f, 0.f),
		new TextureHandle("Tank_0.png"),
		new TankControls(p1_keys),
		new BoundingCircle(),
		new Gun(),
		new Sprite()
	);
	
	

	std::array<sf::Keyboard::Key, 5> p2_keys = {{ sf::Keyboard::D, sf::Keyboard::A, sf::Keyboard::W, sf::Keyboard::S, sf::Keyboard::F }};

	unsigned tank2 = main_env.createEntity(
		new Transform(400.f, 300.f, 0.f),
		new Velocity(0.f, 0.f),
		new TextureHandle("Tank_0.png"),
		new TankControls(p2_keys),
		new BoundingCircle(),
		new Gun(),
		new Sprite()
	);
	
	

	// camera
	sf::FloatRect borders = sf::FloatRect(0.f, 0.f, 64.f * 20.f, 64.f * 12.f);
	sf::FloatRect viewport = sf::FloatRect(0.f, 0.f, 1.f, 1.f);
	float ratio = (float)window->getSize().x/(float)window->getSize().y;
	
	main_env.createEntity(
		new ViewController(borders, viewport, ratio, 400.f, 1200.f, 0.4f, {tank1, tank2})
	);

	// testing button
	main_env.createEntity(
		new Transform(200.f, 30.f, 0.f),
		new MouseControls(),
		/*UserInterface([]()->void* { std::cout<<"button works!"<<std::endl; return nullptr; }, 
					 std::bitset<4>(1<<UserInterface::HIGHLIGHT | 1<<UserInterface::CLICK | 1<<UserInterface::PRESS)),*/
		new GUIObj(GUIObj::BUTTON, []()->void* { std::cout<<"button works"<<std::endl; return nullptr; }),
		new TextureHandle("Button.png"),
		new Sprite()
	);


	// Testing Radio Button
	/*main_env.createEntity(
		Transform(500.f, 30.f, 0.f),
		MouseControls(),
		GUIObj(GUIObj::RADIO, []()->void* { ; return nullptr; }),
		TextureHandle("Button.png")
		);*/

// [FACTORY CONSTRUCTS]
	ui_system = new UISystem(&main_env);
	
	
}

Application::~Application()
{
	delete ui_system;

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
	ui_system->update(&main_env);
	expiring_system.update(&main_env, dt);

	physics_system.update(&main_env, dt);

	texture_manager.update(&main_env);
	map_creation_system.update(&main_env);
	view_system.update(&main_env, window, dt);
	render_system.update(&main_env, window);
}
