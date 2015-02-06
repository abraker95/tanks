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

		enum GAMEMODE
		{
			LOCAL,
			ONLINE
		};

		void NewLocalGame(Environment* _gameEnv, Managers* _mgrs);
		void NewNetGame(Environment* _gameEnv, Managers* _mgrs);
		void EndGame(Environment* _gameEnv, bool _newScore);
		void ResetGame(Environment* _gameEnv, Managers* _mgrs);
		void PauseGame();
		void ResumeGame();

		void playerJoin(Environment* _env, Managers* _mgrs, unsigned _playerNum, bool _localPlayer);
		void playerLeave(Environment* _env, unsigned _player);

		GAMESTATE getGameState() const;
		bool isOnline() const;
		unsigned getPlayer(unsigned _player) const;
		unsigned getPlayerNum(unsigned _player);
		unsigned getNumPlayers();

	private:
		std::vector<std::pair<unsigned, unsigned>> players; // players playing the game (dead or not dead)   <tankInMem, playerNum>
		GAMESTATE gameState;
		GAMEMODE gameMode; // local, online
		
};

