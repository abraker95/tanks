#include <array>
#include <bitset>
#include "systems/input_system.h"
#include "systems/physics_system.h"
#include "Components.h"
#include "events.h"

InputSystem::InputSystem() {}

InputSystem::~InputSystem() {}

void InputSystem::update(Environment* _mainEnv, Environment* _uiEnv, Environment* _gameEnv, EntityManager* entity_manager, TextureManager* texture_manager, CPUManager* _cpuMgr)
{
	auto tank_controls = _gameEnv->get<TankControls>();
	auto velocity = _gameEnv->get<Velocity>();

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

	for(unsigned i = 0; i<_cpuMgr->IDs.size(); i++)
	{
		unsigned int ID = _cpuMgr->IDs[i];
		if(_mainEnv->getEntityName(ID)=="CPU")
		{
			if(input_manager.keyClickState[sf::Keyboard::F2])
				*_mainEnv->get<StdComponent<bool>>()[ID].data = !*_mainEnv->get<StdComponent<bool>>()[ID].data;
		}
	}

	for(unsigned ID = 0; ID<_uiEnv->maxEntities(); ID++)
	{
		if(_uiEnv->hasComponents<UserInterface, GUIObj>(ID))
		{
			auto GUIobjs = _uiEnv->get<GUIObj>();
			if(GUIobjs[ID].type==GUIObj::VOID)
			{
				if(input_manager.keyClickState.test(sf::Keyboard::Escape))
				{
					auto ui = _uiEnv->get<UserInterface>();
					auto visible = _uiEnv->get<StdComponent<bool>>()[ID].data;

					if(ui[ID].action) (*ui[ID].action)();
					_mainEnv->emit(new MenuEvent(*visible));
				}
			}
		}
	}

	/// \TODO: See if it's possible to get rid of the menu event
	auto menuEvent = _mainEnv->getEvents<MenuEvent>();
	static bool updateGameEntities = false;

	if(menuEvent.size()>=1)
		updateGameEntities = !menuEvent[0].menuVisible;

	if(updateGameEntities)
	{
		for(unsigned ID = 0; ID<_gameEnv->maxEntities(); ID++)
		{
			if(_gameEnv->hasComponents<TankControls, Velocity>(ID))
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
					if(_gameEnv->hasComponents<Gun, Transform>(ID))
					{
						auto gun = _gameEnv->get<Gun>();
						auto transform = _gameEnv->get<Transform>();

						if(gun[ID].fireClock.getElapsedTime().asSeconds()>gun[ID].fireCooldown)
						{
							gun[ID].fireClock.restart();
							entity_manager->spawnBullet("", _gameEnv, texture_manager, ID);
						}
					}
				}
			}
		}
	}
}
