#include <array>
#include <bitset>
#include "systems/input_system.h"
#include "systems/physics_system.h"
#include "Components.h"

InputSystem::InputSystem() {}

InputSystem::~InputSystem() {}

void InputSystem::update(Environment* env)
{
	TankControls* tank_controls = env->get<TankControls>();
	Velocity* velocity = env->get<Velocity>();
	Transform* transform = env->get<Transform>();
	MouseControls* mouseControls = env->get<MouseControls>();

	for(unsigned i=0;i<env->maxEntities();i++)
	{
		  if(env->hasComponents<MouseControls/*, UI*/>(i))
		  {
			mouseControls[i].Update();
			std::bitset<3>& state = mouseControls[i].state;

			     if(state.test(MouseControls::LEFT)) /*\TODO: UI event */;
		    else if(state.test(MouseControls::MIDDLE)) /*\TODO: UI event */;
		    else if(state.test(MouseControls::RIGHT)) /*\TODO: UI event */;
			else /*\TODO: UI event */; 
		  }
		

		if(env->hasComponents<TankControls, Velocity, Transform, BoundingCircle>(i))
		{
			// update the keystate bitmask
			std::array<sf::Keyboard::Key,5>& keys = tank_controls[i].keys;
			std::bitset<5>& state = tank_controls[i].state;

			state.reset();
			for(int j = 0; j<5; j++)
				if(sf::Keyboard::isKeyPressed(keys[j]))
					state.set(j, true);

			// update the values based on keystate
			     if(state.test(TankControls::TURN_RIGHT)) velocity[i].vrot = +300.f;
			else if(state.test(TankControls::TURN_LEFT))  velocity[i].vrot = -300.f;
			else										  velocity[i].vrot =    0.f;

			     if(state.test(TankControls::GO_FORWARD))  velocity[i].speed = +300.f;
			else if(state.test(TankControls::GO_BACKWARD)) velocity[i].speed = -300.f;
			else									       velocity[i].speed =    0.f;

			if(state.test(TankControls::FIRE))
			{
				if(env->hasComponents<Gun>(i))
				{
					Gun* gun = env->get<Gun>();

					if(gun[i].fireClock.getElapsedTime().asSeconds() > gun[i].fireCooldown)
					{
						gun[i].fireClock.restart();
						env->createEntity(
							Transform(transform[i].x, transform[i].y, transform[i].rot),
							Velocity(500.f, 0.f),
							TextureHandle("Bullet_0.png"),
							Expires(5.f),
							Projectile(-20, i),
							BoundingCircle()
						);
					}
				}
			}
		}
	}
}
