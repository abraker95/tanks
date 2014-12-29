#pragma once
#include <SFML/Graphics.hpp>
#include "core/environment.h"

class CPUSystem: public Component<CPUSystem>
{
	public:
		CPUSystem(Environment* _env, sf::RenderWindow* _win);
		virtual ~CPUSystem();

		void update(Environment* _env);

	private:
		sf::Text* label;
};

