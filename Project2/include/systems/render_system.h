#pragma once
#include <SFML/Graphics.hpp>
#include "core/environment.h"
#include "managers.h"

class RenderSystem : public Component<RenderSystem>
{
	public:
		RenderSystem(sf::RenderWindow* _win);
		~RenderSystem();

		void update(Environment* _env, sf::RenderWindow* _win, EntityManager* _entMgr, UI_Manager* _uiMgr, MapLoader* _mapLdr);

	private:
		sf::Shader* shader;
		sf::RenderTexture GameScene, UIScene;
		bool fullscreen, prevFullscreen;
};
