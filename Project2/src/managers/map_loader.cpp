#include <iostream>
#include <fstream>
#include "Components.h"
#include "managers/map_loader.h"

MapLoader::MapLoader()
{
}

MapLoader::~MapLoader()
{
	for(unsigned i=0;i<arrays.size();i++)
		delete arrays[i];
}


unsigned MapLoader::createMap(Environment* env, TextureManager* tex_man, std::string filename)
{
	std::string tilesheet;
	char tileWidth, tileHeight;
	char tileCountX, tileCountY;
	char numLayers;
	char* mapData;
	
	if(!readMap(filename, tilesheet, tileWidth, tileHeight, tileCountX, tileCountY, numLayers, mapData))
		return 0;

	sf::Texture* tileset = tex_man->load(tilesheet);

	if(!tileset)
		return 0;

	sf::VertexArray* array = buildVA(
		env, tileset, (int)tileWidth, (int)tileHeight, (int)tileCountX, (int)tileCountY, (int)numLayers, mapData);

	arrays.push_back(array);

	unsigned int id = env->createEntity("map",
		new VertexArray(array),
		new Texture(tileset),
		new Tilemap());

	IDs.push_back(id);
	return id;
}

bool MapLoader::readMap(
	const std::string& filename, 
	std::string& tilesheet,
	char& tileWidth, char& tileHeight,
	char& tileCountX, char& tileCountY,
	char& numLayers,
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

	map.get(numLayers);
	
	mapData = new char[(int)tileCountX * (int)tileCountY * (int)numLayers * 2];
	map.read(mapData, (int)tileCountX * (int)tileCountY * (int)numLayers * 2);

	map.close();

	return true;
}

sf::VertexArray* MapLoader::buildVA(
	Environment* env,
	sf::Texture* tileset,
	int tileWidth, int tileHeight,
	int tileCountX, int tileCountY,
	int numLayers,
	char* mapData)
{
	sf::VertexArray* vertices = new sf::VertexArray;

	// count the number of tiles
	vertices->setPrimitiveType(sf::Quads);
	vertices->resize(tileCountX * tileCountY * 4);

	int quadIndex = 0;
	for(int l=0;l<numLayers;l++)
	{
		char* layerData = mapData + tileCountX * tileCountY * 2 * l;

		// iterating over floats is dangerous, because of imprecision ( Sherushe )
		// for(float i=0;i<tileCountX;i++)
		for(int i=0;i<tileCountX;i++)
		{
			for(int j=0;j<tileCountY;j++)
			{
				int tileType = (int)layerData[(j * tileCountX + i) * 2];
				int tileNumber = (int)layerData[(j * tileCountX + i) * 2 + 1];
				int tu = tileNumber % (tileset->getSize().x / tileWidth);
				int tv = tileNumber / (tileset->getSize().x / tileWidth);
				
				// 1st layer
				if(l == 0 && tileType != 0)
				{
					sf::Vertex* quad = &(*vertices)[quadIndex++ * 4];

					quad[0].position = sf::Vector2f((float)i       	* tileWidth, (float)j	   	* tileHeight);
					quad[1].position = sf::Vector2f((float)(i + 1) 	* tileWidth, (float)j	   	* tileHeight);
					quad[2].position = sf::Vector2f((float)(i + 1) 	* tileWidth, (float)(j + 1) * tileHeight);
					quad[3].position = sf::Vector2f((float)i		* tileWidth, (float)(j + 1) * tileHeight);

					quad[0].texCoords = sf::Vector2f((float)tu		 	* tileWidth, (float)tv		 * tileHeight);
					quad[1].texCoords = sf::Vector2f((float)(tu + 1)	* tileWidth, (float)tv		 * tileHeight);
					quad[2].texCoords = sf::Vector2f((float)(tu + 1) 	* tileWidth, (float)(tv + 1) * tileHeight);
					quad[3].texCoords = sf::Vector2f((float)tu		 	* tileWidth, (float)(tv + 1) * tileHeight);
				}

				// 2nd layer
				if(l == 1 && tileType != 0)
				{
					auto sprites = env->get<Sprite>();

					unsigned new_wall = env->createEntity("",
						new Transform(Vec2f(
							((float)i + 0.5f) * (float)tileWidth, (((float)j + 0.5f) * (float)tileWidth))),
						new Texture(tileset),
						new BoundingBox(Vec2f(tileWidth, tileHeight)),
						new Sprite(),
						new Solid()
					);

					sprites[new_wall].sprite.setOrigin((float)tileWidth/2.f, (float)tileHeight/2.f);
					sprites[new_wall].sprite.setTextureRect(
						sf::IntRect(tu * tileWidth, tv * tileHeight,(tu + 1) * tileWidth, (tv + 1) * tileHeight));
					
					IDs.push_back(new_wall);
				}
			}
		}
	}

	return vertices;
}
