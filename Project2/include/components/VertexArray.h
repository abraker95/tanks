#pragma once
#include <core/environment.h>
#include <SFML/Graphics.hpp>

class VertexArray : public Component<VertexArray>
{
public:
	VertexArray();

	sf::VertexArray vertices;
};
