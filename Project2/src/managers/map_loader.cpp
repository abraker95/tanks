#include <iostream>
#include <fstream>
#include "Components.h"
#include "managers/map_loader.h"

MapLoader::MapLoader()
{
}

MapLoader::~MapLoader()
{
}


unsigned MapLoader::createMap(Environment* env, TextureManager* tex_man, std::string filename)
{
	std::string tilesheet;
	char tileWidth, tileHeight;
	char tileCountX, tileCountY;
	char* mapData;
	
	if(!readMap(filename, tilesheet, tileWidth, tileHeight, tileCountX, tileCountY, mapData))
		return 0;

	sf::Texture* tileset = tex_man->load(tilesheet);

	if(!tileset)
		return 0;

	sf::VertexArray* array = buildVA(
		tileset, (int)tileWidth, (int)tileHeight, (int)tileCountX, (int)tileCountY, mapData);

	return env->createEntity(
		new VertexArray(array),
		new Texture(tileset),
		new Tilemap());
}

bool MapLoader::readMap(
	const std::string& filename, 
	std::string& tilesheet,
	char& tileWidth, char& tileHeight,
	char& tileCountX, char& tileCountY,
	char*& mapData)
{
	ifstream map;
	map.open(filename, ios::in | ios::binary);
	if(!map.is_open())
	{
		std::cout<<"ERROR: Could not load the map "<<filename<<std::endl;
		return false;
	}

	std::getline(map, tilesheet, map.widen('\0'));

	map.get(tileWidth);
	map.get(tileHeight);

	map.get(tileCountX);
	map.get(tileCountY);
	
	mapData = new char[(int)tileCountX * (int)tileCountY * 2];
	map.read(mapData, (int)tileCountX * (int)tileCountY * 2);

	map.close();

	return true;
}

sf::VertexArray* MapLoader::buildVA(
	sf::Texture* tileset,
	int tileWidth, int tileHeight,
	int tileCountX, int tileCountY,
	char* mapData)
{
	sf::VertexArray* vertices = new sf::VertexArray;

	vertices->setPrimitiveType(sf::Quads);
	vertices->resize(tileCountX * tileCountY * 4);

	for(int i=0;i<tileCountX;i++)
	{
		for(int j=0;j<tileCountY;j++)
		{
			int tileNumber = (int)mapData[(j * (int)tileCountX + i) * 2 + 1];

			int tu = tileNumber % (tileset->getSize().x / tileCountX);
			int tv = tileNumber / (tileset->getSize().x / tileCountY);

			sf::Vertex* quad = &(*vertices)[(j * tileCountX + i) * 4];

			quad[0].position = sf::Vector2f(i       * tileWidth, j	   * tileHeight);
			quad[1].position = sf::Vector2f((i + 1) * tileWidth, j	   * tileHeight);
			quad[2].position = sf::Vector2f((i + 1) * tileWidth, (j + 1) * tileHeight);
			quad[3].position = sf::Vector2f(i		* tileWidth, (j + 1) * tileHeight);

			quad[0].texCoords = sf::Vector2f(tu		  * tileWidth, tv		  * tileHeight);
			quad[1].texCoords = sf::Vector2f((tu + 1) * tileWidth, tv		  * tileHeight);
			quad[2].texCoords = sf::Vector2f((tu + 1) * tileWidth, (tv + 1) * tileHeight);
			quad[3].texCoords = sf::Vector2f(tu		  * tileWidth, (tv + 1) * tileHeight);
		}
	}

	return vertices;
}
