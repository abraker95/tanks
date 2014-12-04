#pragma once
#include <SFML/Graphics.hpp>
#include "core/environment.h"

class RenderSystem : public Component<RenderSystem>
{
public:
	RenderSystem();
	~RenderSystem();

	void update(Environment* env, sf::RenderWindow* window);
};
