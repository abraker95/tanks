#pragma once

#include <SFML/Graphics.hpp>
#include <cstring>

struct TextureHandle
{
	TextureHandle();
	TextureHandle(std::string filename);

	std::string filename;
	sf::Texture* texture;
};
