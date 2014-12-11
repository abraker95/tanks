#pragma once

#include <string>
#include <SFML/Graphics.hpp>
#include "core/environment.h"
#include "managers/texture_manager.h"

class MapLoader
{
public:
	MapLoader();
	~MapLoader();

	unsigned createMap(Environment* env, TextureManager* tex_man, std::string filename);

private:
	sf::VertexArray* buildVA(
		sf::Texture* tilesheet,
		int tileWidth, int tileHeight,
		int tileCountX, int tileCountY,
		char* mapData);

	bool readMap(
		const std::string& filename, 
		std::string& tilesheet,
		char& tileWidth, char& tileHeight,
		char& tileCountX, char& tileCountY,
		char*& mapData);
};
