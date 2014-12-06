#pragma once

#include <SFML/Graphics.hpp>
#include "core/environment.h"

struct MapDesc : public Component<MapDesc>
{
	MapDesc(int* map, int width, int height, sf::Vector2u tile_size);

	int* map;
	int width, height;
	sf::Vector2u tile_size;
};
