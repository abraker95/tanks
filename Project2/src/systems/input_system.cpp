#include <array>
#include <bitset>
#include "systems/input_system.h"
#include "systems/physics_system.h"
#include "Components.h"
#include "events.h"
#include "utils.h"

InputSystem::InputSystem() {}

InputSystem::~InputSystem() {}

void InputSystem::update(Environment* _env, sf::RenderWindow* win, EntityManager* entity_manager, TextureManager* texture_manager, UI_Manager* _UImgr)
{
	static sf::Event event;
	while(win->pollEvent(event))
	{
		if(event.type == sf::Event::Closed)
			win->close();
		if(event.type == sf::Event::Resized)
			_env->emit(new ResizeEvent(event.size.width, event.size.height));
	}

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
	input_manager.key = -1;

	for(unsigned short j = 0; j<sf::Keyboard::KeyCount; j++)
	{
		if(sf::Keyboard::isKeyPressed((sf::Keyboard::Key(j))))
		{
			if(prevKeyPressState[j] ^ true) input_manager.keyClickState.set(j, true);
			input_manager.keyPressState.set(j, true);

			if(sf::Keyboard::Key(j)!=sf::Keyboard::Key::RShift && sf::Keyboard::Key(j)!=sf::Keyboard::Key::LShift)
				input_manager.key = j;
		}
	}

	bool supported = BTWN(sf::Keyboard::Key::A, input_manager.key, sf::Keyboard::Key::Z)||
					 BTWN(sf::Keyboard::Key::Num0, input_manager.key, sf::Keyboard::Key::Num9)||
					 input_manager.key==sf::Keyboard::Key::Space||input_manager.key==sf::Keyboard::Key::BackSpace;

	if(input_manager.keyPressState.test(sf::Keyboard::Key::RShift)||input_manager.keyPressState.test(sf::Keyboard::Key::LShift))
	{ 
		if(BTWN(sf::Keyboard::Key::A, input_manager.key, sf::Keyboard::Key::Z))
			input_manager.key += 65; // capital letter
		else if(BTWN(sf::Keyboard::Key::Num0, input_manager.key, sf::Keyboard::Key::Num9))   
			input_manager.key = 0;
			// input_manager.key += 6;  // symbols  /// \TODO (abraker): Nope, no support for symbols until I feel like mapping them
	}
	else
	{
		if(BTWN(sf::Keyboard::Key::A, input_manager.key, sf::Keyboard::Key::Z))
			input_manager.key += 97; // lower letter
		else if(BTWN(sf::Keyboard::Key::Num0, input_manager.key, sf::Keyboard::Key::Num9))
			input_manager.key += 22;  // numbers
	}
	if(input_manager.key==sf::Keyboard::Key::Space) input_manager.key = 32;
	if(!supported) input_manager.key = 0;
	/// \NOTE: keep backspace as for now is to be used with sfml's enum

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
					ui[ID].action();
			}
		}

		if(_env->hasComponents<UserInterface, Label>(ID))
		{
			auto ui = _env->get<UserInterface>();
			auto labels = _env->get<Label>();
			if(ui[ID].state.test(UserInterface::FOCUS))
			{
				sf:string str = labels[ID].label.getString();

				if(ui[ID].state.test(UserInterface::KEY))
				{ 
					if(input_manager.key!=0)
					{
						if(input_manager.key==sf::Keyboard::Key::BackSpace)
							str = str.substr(0, str.size()-1);
						else
							str += input_manager.key;
						labels[ID].label.setString(str);
					}
				}
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
