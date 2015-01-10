#pragma once
#include <SFML/Graphics.hpp>
#include "core/environment.h"
#include "managers.h"

class RenderSystem : public Component<RenderSystem>
{
	public:
		RenderSystem(sf::RenderWindow* _win);
		~RenderSystem();

		void update(Environment* _env, Environment* _uiEnv, Environment* _gameEnv, sf::RenderWindow* _win, CPUManager* _cpuMgr);

	private:
		sf::Shader* shader;
		sf::RenderTexture GameScene, UIScene;
		bool fullscreen, prevFullscreen;
};
