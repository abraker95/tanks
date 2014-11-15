#include "Tilemap.h"

bool Tilemap::load(const char* _tileSheet, sf::Vector2u _tileSize, const int* _tiles, unsigned int _width, unsigned int _height) 
{
	if(!tileset.loadFromFile(_tileSheet))
		return false;

	vertices.setPrimitiveType(sf::Quads);
	vertices.resize(_width * _height * 4);

	for(unsigned int i=0;i<_width;i++)
	{
		for(unsigned int j=0;j<_height;j++)
		{
			int tileNumber = _tiles[j * _width + i];

			int tu = tileNumber % (tileset.getSize().x / _tileSize.x);
			int tv = tileNumber / (tileset.getSize().x / _tileSize.x);

			sf::Vertex* quad = &vertices[(j * _width + i) * 4];

			quad[0].position = sf::Vector2f(i       * _tileSize.x, j	   * _tileSize.y);
			quad[1].position = sf::Vector2f((i + 1) * _tileSize.x, j	   * _tileSize.y);
			quad[2].position = sf::Vector2f((i + 1) * _tileSize.x, (j + 1) * _tileSize.y);
			quad[3].position = sf::Vector2f(i		* _tileSize.x, (j + 1) * _tileSize.y);

			quad[0].texCoords = sf::Vector2f(tu		  * _tileSize.x, tv		  * _tileSize.y);
			quad[1].texCoords = sf::Vector2f((tu + 1) * _tileSize.x, tv		  * _tileSize.y);
			quad[2].texCoords = sf::Vector2f((tu + 1) * _tileSize.x, (tv + 1) * _tileSize.y);
			quad[3].texCoords = sf::Vector2f(tu		  * _tileSize.x, (tv + 1) * _tileSize.y);
		}
	}

	return true;
}

void Tilemap::draw(sf::RenderTarget& _target, sf::RenderStates _states) const
{
	_states.transform *= getTransform();
	_states.texture = &tileset;
	_target.draw(vertices, _states);
}

