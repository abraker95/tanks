#pragma once
#include "core/environment.h"
#include <vector>

struct Managers;

class GameManager
{
	public:
		GameManager();
		~GameManager();

		enum GAMESTATE
		{
			ENDED = 0,  // On title screen
			PAUSED,     // On pause screen
			PLAYING     // playing
		};

		void NewLocalGame(Environment* _gameEnv, Managers* _mgrs);
		void NewNetGame(Environment* _gameEnv, Managers* _mgrs);
		void EndGame(Environment* _gameEnv, bool _newScore);
		void ResetGame(Environment* _gameEnv, Managers* _mgrs);
		void PauseGame();
		void ResumeGame();

		GAMESTATE getGameState() const;
		bool isOnline() const;
		unsigned getPlayer(int _player) const;
		unsigned getNumPlayers();

	private:
		std::vector<unsigned> players; // players playing the game (dead or not dead)
		GAMESTATE gameState;
		bool gameMode; // 0 = local, 1 = online
		
};

