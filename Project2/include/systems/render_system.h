#pragma once
#include <SFML/Graphics.hpp>
#include "core/environment.h"

class RenderSystem : public Component<RenderSystem>
{
	public:
		RenderSystem(sf::RenderWindow* _win);
		~RenderSystem();

		void update(Environment* _env, sf::RenderWindow* _win);

	private:
		sf::Shader* shader;
		sf::RenderTexture GameScene, UIScene;
		bool fullscreen;
};
