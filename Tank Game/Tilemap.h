#pragma once
#include <SFML/Graphics.hpp>

// inspired from this example:
// http://sfml-dev.org/tutorials/2.1/graphics-vertex-array.php#example-tile-map 
class Tilemap : public sf::Drawable, public sf::Transformable
{
public:
	bool Load(const char* _tileSheet, sf::Vector2u _tileSize, const int* _tiles, unsigned int _width, unsigned int _height);

private:
	virtual void draw(sf::RenderTarget& _target, sf::RenderStates _states) const;

	sf::VertexArray vertices;
	sf::Texture tileset;
};
