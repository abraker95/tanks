#pragma once
#include "core/environment.h"
#include <SFML/Graphics.hpp>
#include "math/vector.h"

struct Texture : public Component<Texture>
{
	Texture(sf::Texture* texture);
	Texture(sf::Texture* texture, const sf::IntRect& rect);

	sf::Texture* texture;
	sf::IntRect rect;
};
