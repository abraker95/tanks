#pragma once

#include <array>
#include <SFML/Graphics.hpp>
#include "core/environment.h"
#include "managers/texture_manager.h"

class EntityManager
{
public:
	EntityManager();
	~EntityManager();

	unsigned spawnTankPlayer(
		Environment* env, TextureManager* tex_man, 
		float x, float y, 
		std::array<sf::Keyboard::Key, 5> keys);

	unsigned spawnBullet(
		Environment* env, TextureManager* tex_man, 
		unsigned tank_id);

	unsigned createCamera(
		Environment* env, 
		sf::FloatRect& borders, sf::FloatRect& viewport, 
		std::vector<unsigned> focusedObjects);
};
