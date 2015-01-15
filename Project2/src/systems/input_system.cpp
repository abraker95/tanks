#include <array>
#include <bitset>
#include "systems/input_system.h"
#include "systems/physics_system.h"
#include "Components.h"
#include "events.h"

InputSystem::InputSystem() {}

InputSystem::~InputSystem() {}

void InputSystem::update(Environment* _env, EntityManager* entity_manager, TextureManager* texture_manager, CPUManager* _cpuMgr, UI_Manager* _UImgr)
{
	auto tank_controls = _env->get<TankControls>();
	auto velocity = _env->get<Velocity>();

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

	for(unsigned ID = 0; ID<_env->maxEntities(); ID++)
	{
		if(_env->getEntityName(ID)=="CPU")
		{
			if(input_manager.keyClickState[sf::Keyboard::F2])
				*_env->get<StdComponent<bool>>()[ID].data = !*_env->get<StdComponent<bool>>()[ID].data;
		}
	}

	bool updateGameEntities = false;
	for(unsigned ID = 0; ID<_env->maxEntities(); ID++)
	{
		if(_env->hasComponents<UserInterface, GUIObj>(ID))
		{
			auto GUIobjs = _env->get<GUIObj>();
			if(GUIobjs[ID].type==GUIObj::VOID)
			{
				auto ui = _env->get<UserInterface>();
				updateGameEntities = _UImgr->isVisible(&ui[ID]);

				if(input_manager.keyClickState.test(sf::Keyboard::Escape))
					if(ui[ID].action) (*ui[ID].action)();
			}
		}
	}

	if(updateGameEntities)
	{
		for(unsigned ID = 0; ID<_env->maxEntities(); ID++)
		{
			if(_env->hasComponents<TankControls, Velocity>(ID))
			{
				// update the keystate bitmask
				std::array<sf::Keyboard::Key, 5>& keys = tank_controls[ID].keys;
				std::bitset<5>& state = tank_controls[ID].state;

				state.reset();
				for(int j = 0; j<5; j++)
					if(sf::Keyboard::isKeyPressed(keys[j]))
						state.set(j, true);

				// update the values based on keystate
				if(state.test(TankControls::TURN_RIGHT))	  velocity[ID].vrot = +300.f;
				else if(state.test(TankControls::TURN_LEFT))  velocity[ID].vrot = -300.f;
				else										  velocity[ID].vrot = 0.f;

				if(state.test(TankControls::GO_FORWARD))	   velocity[ID].speed = +300.f;
				else if(state.test(TankControls::GO_BACKWARD)) velocity[ID].speed = -300.f;
				else									       velocity[ID].speed = 0.f;

				if(state.test(TankControls::FIRE))
				{
					if(_env->hasComponents<Gun, Transform>(ID))
					{
						auto gun = _env->get<Gun>();
						auto transform = _env->get<Transform>();

						if(gun[ID].fireClock.getElapsedTime().asSeconds()>gun[ID].fireCooldown)
						{
							gun[ID].fireClock.restart();
							entity_manager->spawnBullet("", _env, texture_manager, ID);
						}
					}
				}
			}
		}
	}
}
