#pragma once
#include <SFML/Graphics.hpp>
#include "core/environment.h"
#include "systems/HUD_System.h"
#include "managers.h"

class RenderSystem : public Component<RenderSystem>
{
	public:
		RenderSystem(sf::RenderWindow* _win);
		~RenderSystem();

		void update(Environment* _env, HUDSystem* _HUDSystem, sf::RenderWindow* _win, CPUManager* _cpuMgr);

	private:
		sf::Shader* shader;
		sf::RenderTexture GameScene, UIScene;
		bool fullscreen, prevFullscreen;
};
