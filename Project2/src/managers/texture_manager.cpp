#include <iostream>
#include "managers/texture_manager.h"

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
	for(auto it=texture_atlas.begin();it!=texture_atlas.end();it++)
	{
		delete it->second;
	}

	texture_atlas.clear();
}

sf::Texture* TextureManager::load(std::string filename)
{
	return lookupAtlas(filename);
}

sf::Texture* TextureManager::lookupAtlas(const std::string& filename)
{
	auto it = texture_atlas.find(filename);
	if(it != texture_atlas.end())
		return it->second;
	
	sf::Texture* new_texture = new sf::Texture;
	if(!new_texture->loadFromFile(filename))
	{
		std::cout<<"ERROR: Could not load "<<filename<<std::endl;
		return nullptr;
	}

	texture_atlas[filename] = new_texture;
	return new_texture;
}
