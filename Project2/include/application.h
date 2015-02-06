#pragma once
#include <SFML/Graphics.hpp>
#include "core/environment.h"
#include "Systems.h"
#include "managers/managers.h"

class Application
{
public:
	Application();
	~Application();

	int run();	
	void update(float dt);

private:
	sf::Clock runTime;
	sf::RenderWindow* window;
	bool fullscreen;

	// Environments
	Environment mainEnv;

	// Systems
	InputSystem* input_system;
	RenderSystem* render_system;
	ExpiringSystem* expiring_system;
	PhysicsSystem* physics_system;
	UISystem* ui_system;
	ViewSystem* view_system;
	DamageSystem* damage_system;
	HUDSystem* hud_system;
	ScoreSystem* score_system;
	NetworkSystem* net_system;
	
	// Managers
	Managers managers;
};
