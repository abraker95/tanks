#pragma once

#include <array>
#include <SFML/Graphics.hpp>
#include "core/environment.h"
#include "managers/texture_manager.h"
#include "managers/score_manager.h"

class EntityManager
{
	public:
		EntityManager();
		~EntityManager();

		unsigned spawnTankPlayer(std::string _name,
			Environment* _gameEnv, TextureManager* texture_manager,
			ScoreManager* score_manager, std::array<sf::Keyboard::Key, 5> keys);
	
		void killPlayer(Environment* env, unsigned id);
		void revivePlayer(Environment* env, unsigned id);
		int getNumLivingTanks();
		void placeOnSpawn(Environment* env, unsigned tank_id, unsigned player_id);

		unsigned spawnBullet(std::string _name,
			Environment* _gameEnv, TextureManager* tex_man,
			unsigned tank_id);

		unsigned createCamera(std::string _name,
			Environment* _gameEnv,
			sf::FloatRect& borders, sf::FloatRect& viewport, 
			std::vector<unsigned> focusedObjects);

		//void NewGame(Environment* _gameEnv, TextureManager* texture_manager, ScoreManager* score_manager);
		//void EndGame(Environment* _gameEnv, bool _newScore);
		//void ResetGame(Environment* _gameEnv);

	private:
		int numLivingTanks;
};
