#include "managers/game_manager.h"
#include "Components.h"
#include "Events.h"
#include "managers/managers.h"

GameManager::GameManager()
{
	gameState = ENDED;
}

GameManager::~GameManager()
{}

GameManager::GAMESTATE GameManager::getGameState() const
{
	return gameState;
}

void GameManager::NewLocalGame(Environment* _env, Managers* _mgrs)
{
	// double-braces init because of std::array
	std::array<sf::Keyboard::Key, 5> p1_keys = {{sf::Keyboard::Right, sf::Keyboard::Left, sf::Keyboard::Up, sf::Keyboard::Down, sf::Keyboard::Space}};
	unsigned tank1 = _mgrs->entity_manager.spawnTankPlayer("tank1", _env, &_mgrs->texture_manager, &_mgrs->score_manager, p1_keys);
	_env->get<Label>()[tank1].label.setString("Player 1");
	players.push_back(tank1);

	std::array<sf::Keyboard::Key, 5> p2_keys = {{sf::Keyboard::D, sf::Keyboard::A, sf::Keyboard::W, sf::Keyboard::S, sf::Keyboard::F}};
	unsigned tank2 = _mgrs->entity_manager.spawnTankPlayer("tank2", _env, &_mgrs->texture_manager, &_mgrs->score_manager, p2_keys);
	_env->get<Label>()[tank2].label.setString("Player 2");
	players.push_back(tank2);

	// camera
	sf::FloatRect borders = sf::FloatRect(0.f, 0.f, 64.f * 20.f, 64.f * 20.f);
	sf::FloatRect viewport = sf::FloatRect(0.f, 0.f, 1.f, 1.f);
	_mgrs->entity_manager.createCamera("mainCamera", _env, borders, viewport, {tank1, tank2});

	gameMode = 0; // local
	gameState = PLAYING;
}

void GameManager::NewNetGame(Environment* _env, Managers* _mgrs)
{
	// Create 1 tank only
	// double-braces init because of std::array
	std::array<sf::Keyboard::Key, 5> p1_keys = {{sf::Keyboard::Right, sf::Keyboard::Left, sf::Keyboard::Up, sf::Keyboard::Down, sf::Keyboard::Space}};
	unsigned tank1 = _mgrs->entity_manager.spawnTankPlayer("tank1", _env, &_mgrs->texture_manager, &_mgrs->score_manager, p1_keys);
	_env->get<Label>()[tank1].label.setString("Player 1");
	players.push_back(tank1);

	// camera
	sf::FloatRect borders = sf::FloatRect(0.f, 0.f, 64.f * 20.f, 64.f * 20.f);
	sf::FloatRect viewport = sf::FloatRect(0.f, 0.f, 1.f, 1.f);
	_mgrs->entity_manager.createCamera("mainCamera", _env, borders, viewport, {tank1});

	gameMode = 1; // online
	gameState = PLAYING;
}

void GameManager::playerJoin(Environment* _env, Managers* _mgrs)
{
	std::array<sf::Keyboard::Key, 5> keys = {{sf::Keyboard::Unknown, sf::Keyboard::Unknown, sf::Keyboard::Unknown, sf::Keyboard::Unknown, sf::Keyboard::Unknown}};
	unsigned tank = _mgrs->entity_manager.spawnTankPlayer("tank"+to_string(getNumPlayers()+1), _env, &_mgrs->texture_manager, &_mgrs->score_manager, keys);
	_env->get<Label>()[tank].label.setString("Player "+to_string(getNumPlayers()+1));
	players.push_back(tank);
}

void GameManager::playerLeave(Environment* _env, int _player)
{
	_env->destroyEntity(_env->getID("tank"+to_string(_player)));
	players.erase(players.begin()+_player);
}

void GameManager::PauseGame()
{
	if(gameState!=GAMESTATE::ENDED)
		gameState = PAUSED;
}

void GameManager::ResumeGame()
{
	if(gameState != GAMESTATE::ENDED)
		gameState = PLAYING;
}

void GameManager::ResetGame(Environment* _env, Managers* _mgrs)
{
	auto health = _env->get<Health>();
	auto player = _env->get<Player>();

	for(unsigned id = 0; id<_env->maxEntities(); id++)
	{
		if(_env->hasComponents<Player>(id))
		{
			if(!health[id].hasHealth())
			{
				_mgrs->entity_manager.revivePlayer(_env, id);
				_env->emit(new CreateEvent(id));
			}

			health[id].resetHealth();
			_mgrs->entity_manager.placeOnSpawn(_env, id, player[id].player_id);
		}
	}

	gameState = PLAYING;
}

void GameManager::EndGame(Environment* _env, bool _newScore)
{
	_env->destroyEntity(_env->getID("tank1"));
	_env->destroyEntity(_env->getID("tank2"));
	_env->destroyEntity(_env->getID("mainCamera"));

	players.clear();
	if(gameMode==1) gameMode = 0;
	gameState = ENDED;
}

bool GameManager::isOnline() const
{
	return gameMode;
}

unsigned GameManager::getPlayer(int _player) const
{
	if(_player-1 < players.size())
		return players[_player-1];
}
unsigned GameManager::getNumPlayers()
{
	return players.size();
}
