#include <array>
#include <bitset>
#include "systems/input_system.h"
#include "components/tank_controls.h"
#include "components/velocity.h"

InputSystem::InputSystem()
{
}

InputSystem::~InputSystem()
{
}

void InputSystem::update(Environment* env)
{
	TankControls* tank_controls = env->get<TankControls>();
	Velocity* velocity = env->get<Velocity>();

	for(unsigned i=0;i<env->maxEntities();i++)
	{
		if(env->hasComponents<TankControls, Velocity>(i))
		{
			std::array<sf::Keyboard::Key, 5>& keys = tank_controls[i].keys;
			std::bitset<5>& state = tank_controls[i].state;

			state.reset();
			for(int j=0;j<5;j++)
				if(sf::Keyboard::isKeyPressed(keys[j]))
					state.set(j, true);

			if(state.test(TankControls::TURN_RIGHT))
				velocity[i].vrot = 300.f;
			else if(state.test(TankControls::TURN_LEFT))
				velocity[i].vrot = -300.f;
			else
				velocity[i].vrot = 0.f;

			if(state.test(TankControls::GO_FORWARD))
				velocity[i].speed = 300.f;
			else if(state.test(TankControls::GO_BACKWARD))
				velocity[i].speed = -300.f;
			else
				velocity[i].speed = 0.f;

			if(state.test(TankControls::FIRE))
			{
				// spawn bullet
				// env->createEntity(...);
			}

		}
	}
}
