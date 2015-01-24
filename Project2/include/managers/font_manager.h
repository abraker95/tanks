#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <utility>
#include <string>

class FontManager
{
public:
	FontManager();
	~FontManager();

	const sf::Font& load(std::string filename);

private:
	std::vector<std::pair<std::string, sf::Font*>> fonts;
};
