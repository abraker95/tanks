#pragma once
#include <SFML/Graphics.hpp>
#include "core/environment.h"
#include "systems/input_system.h"
#include "systems/texture_manager.h"
#include "systems/render_system.h"
#include "systems/movement_system.h"
#include "systems/expiring_system.h"

class Application
{
public:
	Application();
	~Application();

	int run();	
	void update(float dt);

private:
	sf::RenderWindow* window;

	// Environments
	Environment main_env;

	// Systems
	InputSystem input_system;
	TextureManager texture_manager;
	RenderSystem render_system;
	MovementSystem movement_system;
	ExpiringSystem expiring_system;
};
