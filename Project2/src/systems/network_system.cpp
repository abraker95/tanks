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
		if(checkForIncomingPlayers())
		{
			/// increase amount of tanks by 1
		}

		// getPlayerInfo();
		// sendPlayerInfo();
	}
}

unsigned NetworkSystem::checkForIncomingPlayers()
{
	/// if there are incoming players, get IP addresses and add them to client IP list
	/// if there are no incoming players, return 0 (0.0.0.0)
	return 0;
}
