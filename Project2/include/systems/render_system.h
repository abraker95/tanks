#pragma once
#include <SFML/Graphics.hpp>
#include "core/environment.h"

class RenderSystem : public Component<RenderSystem>
{
	public:
		RenderSystem(sf::RenderWindow* window);
		~RenderSystem();

		void update(Environment* env, sf::RenderWindow* _win, sf::Font* font);
};
