#pragma once
#include "core/environment.h"

class HUDSystem
{
	public:
		HUDSystem(Environment* _env);
		~HUDSystem();

		void update(Environment* _env, sf::Window* _win, sf::RenderTexture& _HUDScene);

	private:
		sf::RectangleShape bar;
		const static unsigned numTanks = 2;
		unsigned IDs[numTanks];
		sf::Clock clock;

		sf::Vector2i View2ScreenCor(sf::Vector2i _viewCor, sf::View* _view, sf::Window* _win);
};	

