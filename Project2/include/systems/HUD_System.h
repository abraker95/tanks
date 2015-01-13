#pragma once
#include "core/environment.h"

class HUDSystem
{
	public:
		HUDSystem(Environment* _env);
		~HUDSystem();

		void update(Environment* _env, sf::RenderTexture& _HUDScene);

	private:
		sf::RectangleShape bar;
		const static unsigned numTanks = 2;
		unsigned IDs[numTanks];
		sf::Clock clock;
};	

