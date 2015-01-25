#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include "core/environment.h"
#include "systems/HUD_System.h"
#include "managers/managers.h"

class RenderSystem : public System
{
	public:
		RenderSystem(sf::RenderWindow* _win, FontManager* font_manager);
		~RenderSystem();

		void update(Environment* _env, HUDSystem* _HUDSystem, Managers* managers, sf::RenderWindow* _win, const std::string& monitoring_results);

	private:
		sf::Shader* shader;
		sf::RenderTexture GameScene, UIScene;
		sf::Text debug_infos;
		bool fullscreen, prevFullscreen;
};
