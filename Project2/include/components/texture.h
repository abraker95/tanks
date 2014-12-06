#pragma once
#include "core/environment.h"
#include <SFML/Graphics.hpp>

struct Texture : public Component<Texture>
{
	Texture(sf::Texture* texture);

	sf::Texture* texture;
};
