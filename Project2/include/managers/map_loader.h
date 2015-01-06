#pragma once

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "core/environment.h"
#include "managers/texture_manager.h"

class MapLoader
{
	friend class RenderSystem;

	public:
		MapLoader();
		~MapLoader();

		unsigned createMap(Environment* env, TextureManager* tex_man, std::string filename);

	private:
		sf::VertexArray* buildVA(
			Environment* env,
			sf::Texture* tilesheet,
			int tileWidth, int tileHeight,
			int tileCountX, int tileCountY,
			int numLayers,
			char* mapData);

		bool readMap(
			const std::string& filename, 
			std::string& tilesheet,
			char& tileWidth, char& tileHeight,
			char& tileCountX, char& tileCountY,
			char& numLayers,
			char*& mapData);
	
		std::vector<sf::VertexArray*> arrays;
		std::vector<unsigned int> IDs;
};
