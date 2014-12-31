#pragma once
#include <SFML/Graphics.hpp>
#include "core/environment.h"

class CPUManager
{
	public:
		CPUManager();
		virtual ~CPUManager();

		void createCPUMgr(Environment* _env, sf::RenderWindow* _win);
		sf::Time update();

	private:
		sf::Clock clock;
		sf::Text* label;
};

