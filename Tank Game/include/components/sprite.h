#pragma once

#include <SFML/Graphics.hpp>
#include "core/environment.h"

struct RenderProperties : public Component<RenderProperties>
{
	Sprite();
	Sprite();

	sf::Sprite sprite;
};
