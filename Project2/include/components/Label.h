#pragma once
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <string>
#include "core/environment.h"

class Label: public Component<Label>
{
	public:
		Label(std::string _text);
		~Label();

		sf::Text label;
};

