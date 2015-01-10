#pragma once
#include <SFML/Graphics.hpp>
#include "core/environment.h"

class CPUManager
{
	friend class RenderSystem;
	friend class InputSystem;

	public:
		CPUManager();
		virtual ~CPUManager();

		void createCPUMgr(Environment* _mainEnv, Environment* _gameEnv, Environment* _uiEnv, sf::RenderWindow* _win);
		sf::Time update();

	private:
		sf::Clock clock;
		sf::Text* label;
		std::vector<unsigned int> IDs;
};

