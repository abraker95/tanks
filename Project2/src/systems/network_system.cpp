#include "systems/network_system.h"
#include "Components.h"

NetworkSystem::NetworkSystem()
{}


NetworkSystem::~NetworkSystem()
{}

void NetworkSystem::update(Environment* _env, Managers* _mgrs)
{
	if(_mgrs->game_manager.isOnline())
	{
		if(_mgrs->net_manager.isHost())
		{
			if(_mgrs->net_manager.checkForIncomingPlayers(_mgrs))
			{
				_mgrs->game_manager.playerJoin(_env, _mgrs);
			}
		}

		if(_mgrs->game_manager.getNumPlayers()>1)
		{
			_mgrs->net_manager.sendPlayerInfo(_env);
			_mgrs->net_manager.getPlayerInfo(_env);
		}
	}
}
