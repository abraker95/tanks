#include <array>
#include <bitset>
#include "systems/input_system.h"
#include "systems/physics_system.h"
#include "Components.h"
#include "events.h"

InputSystem::InputSystem() {}

InputSystem::~InputSystem() {}

void InputSystem::update(Environment* env, EntityManager* entity_manager, TextureManager* texture_manager)
{
	auto tank_controls = env->get<TankControls>();
	auto velocity = env->get<Velocity>();

/// \TODO: This is taking up too much CPU time. Think of another solution. Consider making it a bitfield.

	// update the mouse state
	std::bitset<3> prevMousePressState =  input_manager.mousePressState; // not a ref; copy
	input_manager.mousePressState.reset();  input_manager.mouseClickState.reset();
			 
	for(int j = 0; j<3; j++)
	{
		if(sf::Mouse::isButtonPressed(input_manager.buttons[j]))
		{
			if(prevMousePressState[j]^true) input_manager.mouseClickState.set(j, true);
			input_manager.mousePressState.set(j, true);
		}
	}
		
	// update the key states  
	std::bitset<sf::Keyboard::KeyCount> prevKeyPressState = input_manager.keyPressState; // not a ref
	input_manager.keyPressState.reset();  input_manager.keyClickState.reset();
	
	for(int j = 0; j<sf::Keyboard::KeyCount; j++)
	{
		if(sf::Keyboard::isKeyPressed((sf::Keyboard::Key(j))))
		{
			if(prevKeyPressState[j] ^ true) input_manager.keyClickState.set(j, true);
			input_manager.keyPressState.set(j, true);
		}
	}

	for(unsigned i = 0; i<env->maxEntities(); i++)
	{
		if(env->getEntityName(i)=="CPU")
		{
			if(input_manager.keyClickState[sf::Keyboard::F2])
				*env->get<StdComponent<bool>>()[i].data = !*env->get<StdComponent<bool>>()[i].data;
		}
	}

	auto menuEvent = env->getEvents<MenuEvent>();
	static bool updateGameEntities = false;

	if(menuEvent.size()>=1)
		updateGameEntities = !menuEvent[0].menuVisible;

	if(updateGameEntities)
	{
		for(unsigned i = 0; i<env->maxEntities(); i++)
		{
			if(env->hasComponents<TankControls, Velocity>(i))
			{
				// update the keystate bitmask
				std::array<sf::Keyboard::Key, 5>& keys = tank_controls[i].keys;
				std::bitset<5>& state = tank_controls[i].state;

				state.reset();
				for(int j = 0; j<5; j++)
					if(sf::Keyboard::isKeyPressed(keys[j]))
						state.set(j, true);

				// update the values based on keystate
				if(state.test(TankControls::TURN_RIGHT))	  velocity[i].vrot = +300.f;
				else if(state.test(TankControls::TURN_LEFT))  velocity[i].vrot = -300.f;
				else										  velocity[i].vrot = 0.f;

				if(state.test(TankControls::GO_FORWARD))	   velocity[i].speed = +300.f;
				else if(state.test(TankControls::GO_BACKWARD)) velocity[i].speed = -300.f;
				else									       velocity[i].speed = 0.f;

				if(state.test(TankControls::FIRE))
				{
					if(env->hasComponents<Gun, Transform>(i))
					{
						auto gun = env->get<Gun>();
						auto transform = env->get<Transform>();

						if(gun[i].fireClock.getElapsedTime().asSeconds()>gun[i].fireCooldown)
						{
							gun[i].fireClock.restart();
							entity_manager->spawnBullet("", env, texture_manager, i);
						}
					}
				}
			}
		}
	}
}
