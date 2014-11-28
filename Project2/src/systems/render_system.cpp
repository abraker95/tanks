#include "components/transform.h"
#include "components/render_properties.h"
#include "systems/render_system.h"

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
	}

	window->display();
}
