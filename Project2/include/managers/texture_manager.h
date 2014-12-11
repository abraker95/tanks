#pragma once

#include <string>
#include <map>
#include <SFML/Graphics.hpp>

class TextureManager
{
public:
	TextureManager();
	~TextureManager();

	sf::Texture* load(std::string filename);

private:
	sf::Texture* lookupAtlas(const std::string& filename);
	std::map<std::string, sf::Texture*> texture_atlas;
};
