#pragma once
#include "core/environment.h"
#include <SFML/Graphics.hpp>

struct Sprite : public Component<Sprite>
{
	Sprite();

	sf::Sprite sprite;
};
