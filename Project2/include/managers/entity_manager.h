#pragma once

#include <array>
#include <SFML/Graphics.hpp>
#include "core/environment.h"
#include "managers/texture_manager.h"

class EntityManager
{
	friend class RenderSystem;
	friend class InputSystem;

	public:
		EntityManager();
		~EntityManager();

		unsigned spawnTankPlayer(std::string _name,
			Environment* _gameEnv, TextureManager* tex_man,
			float x, float y, 
			std::array<sf::Keyboard::Key, 5> keys);

		unsigned spawnBullet(std::string _name,
			Environment* _gameEnv, TextureManager* tex_man,
			unsigned tank_id);

		unsigned createCamera(std::string _name,
			Environment* _gameEnv,
			sf::FloatRect& borders, sf::FloatRect& viewport, 
			std::vector<unsigned> focusedObjects);

		void NewGame(Environment* _gameEnv, TextureManager* _textmgr);
		void EndGame(Environment* _gameEnv, bool _newScore);
		void ResetGame(Environment* _gameEnv, TextureManager* _textmgr, bool _newScore);

	private:
		std::vector<unsigned int> IDs;
};
