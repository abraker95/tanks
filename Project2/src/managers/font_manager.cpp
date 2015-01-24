#include <iostream>

#include "managers/font_manager.h"

FontManager::FontManager()
{
}

FontManager::~FontManager()
{
	for(auto font : fonts)
		delete font.second;
	fonts.clear();
}

const sf::Font& FontManager::load(std::string filename)
{
	for(auto font : fonts)
		if(font.first == filename)
			return *(font.second);
	
	sf::Font* font = new sf::Font;
	if(!font->loadFromFile(filename))
	{
		std::cout<<"Error(FontManager): "<<filename<<" not found"<<std::endl;
		delete font;
		font = nullptr;
	}

	// even when the font fails to load
	// it is put in the "fonts" vector so
	// that it doesn't attempt to load it again
	fonts.push_back(std::make_pair(filename, font));

	// but will probably fail here...
	return *font;
}
