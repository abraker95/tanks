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
	_mgrs->map_loader.loadMap(_env, &_mgrs->texture_manager, "maps/dev1.map");

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

	gameMode = LOCAL;
	gameState = PLAYING;
}

void GameManager::NewNetGame(Environment* _env, Managers* _mgrs)
{
	_mgrs->map_loader.loadMap(_env, &_mgrs->texture_manager, "maps/dev1.map");

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

	gameMode = ONLINE;
	gameState = PLAYING;
}

void GameManager::playerJoin(Environment* _env, Managers* _mgrs)
{
	std::array<sf::Keyboard::Key, 5> keys = {{sf::Keyboard::Unknown, sf::Keyboard::Unknown, sf::Keyboard::Unknown, sf::Keyboard::Unknown, sf::Keyboard::Unknown}};

	unsigned tank = _mgrs->entity_manager.spawnTankPlayer("tank"+to_string(getNumPlayers()+1), _env, &_mgrs->texture_manager, &_mgrs->score_manager, keys);

	_env->emit(new CreateEvent(tank));
	players.push_back(tank);
}

void GameManager::playerLeave(Environment* _env, int _player)
{
	_env->emit(new DestroyEvent(_env->getID("tank"+to_string(_player))));
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
		}
		
		else if(!_env->hasComponents<ViewController>(id) && !_env->hasComponents<GUIObj>(id))
		{
			_env->destroyEntity(id);
		}
	}

	_mgrs->map_loader.reloadMap(_env, &_mgrs->texture_manager);

	for(unsigned id = 0;id < _env->maxEntities();id++)
	{
		if(_env->hasComponents<Player>(id))
		{
			_mgrs->entity_manager.placeOnSpawn(_env, id, player[id].player_id);
		}
	}

	gameState = PLAYING;
}

void GameManager::EndGame(Environment* _env, bool _newScore)
{
	for(unsigned i=0;i<_env->maxEntities();i++)
	{
		if(!_env->hasComponents<GUIObj>(i))
		{
			_env->emit(new DestroyEvent(i));
			_env->destroyEntity(i);
		}
	}

	players.clear();
	if(gameMode==ONLINE) gameMode = LOCAL;
	gameState = ENDED;
}

bool GameManager::isOnline() const
{
	return gameMode == ONLINE;
}

// returns player 1, player 2, player....   Player 1 is ALWAYS the local player
unsigned GameManager::getPlayer(int _player) const
{
	if(_player-1 < players.size() && _player > 0)
		return players[_player-1];
	else
		cout<<"[GAME MGR] Unable to get player "<<_player<<endl;
}

unsigned GameManager::getNumPlayers()
{
	return players.size();
}
