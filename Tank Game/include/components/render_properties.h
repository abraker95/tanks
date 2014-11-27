#pragma once

#include <SFML/Graphics.hpp>
#include "core/environment.h"

struct RenderProperties : public Component<RenderProperties>
{
	RenderProperties();
	RenderProperties(sf::Texture* texture);
	~RenderProperties();

	sf::Sprite sprite;
};
