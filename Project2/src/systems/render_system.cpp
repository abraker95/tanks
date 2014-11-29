#include "Components.h"
#include "systems/render_system.h"
#include "utils.h"

RenderSystem::RenderSystem()
{
}

RenderSystem::~RenderSystem()
{
}

void RenderSystem::update(Environment* env, sf::RenderWindow* window)
{
	Transform* trans = env->get<Transform>();
	RenderProperties* props = env->get<RenderProperties>();

	window->clear();

	for(unsigned i=0;i<env->maxEntities();i++)
	{
		if(env->hasComponents<Transform, RenderProperties>(i))
		{
			sf::Sprite& sprite = props[i].sprite;
			sprite.setPosition(trans[i].x, trans[i].y);
			sprite.setRotation(trans[i].rot);
			window->draw(sprite);
		}

		if(env->hasComponents<Transform, UserInterface, MouseControls, RenderProperties>(i))
		{
			/// \TODO: Finish button highlighting
			MouseControls* mouse = env->get<MouseControls>();

			sf::FloatRect bounds = props[i].sprite.getLocalBounds();

			if(BTWN(bounds.left, mouse[i].pos[0], bounds.width) && BTWN(bounds.top, mouse[i].pos[1], bounds.height))
			{}
		}
	}

	window->display();
}
