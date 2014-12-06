#include "systems/MapCreationSystem.h"
#include "components/map_desc.h"
#include "components/VertexArray.h"
#include "components/texture.h"
#include "components/tilemap.h"
#include "components/ViewController.h"
#include "components/transform.h"

void MapCreationSystem::update(Environment* env)
{
	for(unsigned i=0;i<env->maxEntities();i++)
	{
		if(env->hasComponents<MapDesc, Texture>(i))
		{
			buildVA(env, i);			
		}
	}
}

void MapCreationSystem::buildVA(Environment* env, int id)
{
	auto vertex_array = env->get<VertexArray>();
	auto map_desc = env->get<MapDesc>();
	auto textures = env->get<Texture>();
	
	env->addComponents<VertexArray>(id, new VertexArray());

	sf::VertexArray& vertices = vertex_array[id].vertices;
	int* tiles = map_desc[id].map;
	unsigned width = map_desc[id].width;
	unsigned height = map_desc[id].height;
	sf::Vector2u& tileSize = map_desc[id].tile_size;
	sf::Texture* tileset = textures[id].texture;

	vertices.setPrimitiveType(sf::Quads);
	vertices.resize(width * height * 4);

	for(unsigned int i=0;i<width;i++)
	{
		for(unsigned int j=0;j<height;j++)
		{
			int tileNumber = tiles[j * width + i];

			int tu = tileNumber % (tileset->getSize().x / tileSize.x);
			int tv = tileNumber / (tileset->getSize().x / tileSize.x);

			sf::Vertex* quad = &vertices[(j * width + i) * 4];

			quad[0].position = sf::Vector2f(i       * tileSize.x, j	   * tileSize.y);
			quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j	   * tileSize.y);
			quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
			quad[3].position = sf::Vector2f(i		* tileSize.x, (j + 1) * tileSize.y);

			quad[0].texCoords = sf::Vector2f(tu		  * tileSize.x, tv		  * tileSize.y);
			quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv		  * tileSize.y);
			quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
			quad[3].texCoords = sf::Vector2f(tu		  * tileSize.x, (tv + 1) * tileSize.y);
		}
	}

	delete[] tiles;
	env->removeComponents<MapDesc>(id);

	// adding a flag component
	env->addComponents<Tilemap>(id, new Tilemap());
}
