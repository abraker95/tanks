#pragma once
#include <SFML/Graphics.hpp>
#include "core/environment.h"
#include "managers/managers.h"

class HUDSystem : public System
{
	public:
		HUDSystem(Environment* _env);
		~HUDSystem();

		void update(Environment* _env, Managers* managers, sf::Window* _win, sf::RenderTexture& _HUDScene);

	private:
		sf::RectangleShape bar;
		const static unsigned numTanks = 2;
		unsigned tankIDs[numTanks], scoreIDs[numTanks];
		sf::Clock clock;

		sf::Vector2i View2ScreenCor(sf::Vector2i _viewCor, sf::View* _view, sf::Window* _win);
};	

