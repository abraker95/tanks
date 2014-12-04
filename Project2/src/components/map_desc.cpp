#include "components/map_desc.h"

MapDesc::MapDesc() : map(nullptr), width(0), height(0)
{
}

MapDesc::MapDesc(int* map, int width, int height, sf::Vector2u tile_size) : map(map), width(width), height(height), tile_size(tile_size)
{
}
