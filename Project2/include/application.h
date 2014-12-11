#pragma once
#include <SFML/Graphics.hpp>
#include "core/environment.h"
#include "Systems.h"
#include "Managers.h"

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
	RenderSystem render_system;
	ExpiringSystem expiring_system;
	PhysicsSystem physics_system;
	UISystem* ui_system;
	ViewSystem view_system;

	// Managers
	EntityManager entity_manager;
	TextureManager texture_manager;
	MapLoader map_loader;
};
