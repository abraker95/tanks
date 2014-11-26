#pragma once

#include <SFML/Graphics.hpp>
#include <cstring>
#include "core/environment.h"

struct TextureHandle : public Component<TextureHandle>
{
	TextureHandle();
	TextureHandle(std::string filename);

	std::string filename;
	sf::Texture* texture;
};
