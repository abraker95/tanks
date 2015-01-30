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
		unsigned players;
		NetManager::PLAYERDATA data;
		auto trans = _env->get<Transform>();

		if(_mgrs->net_manager.isHost())
		{
			if(_mgrs->net_manager.checkForIncomingPlayers(_mgrs))
			{
				cout<<"A player joined the game!"<<endl;
				_mgrs->game_manager.playerJoin(_env, _mgrs);

				/// \TODO: Broadcast new player to clients
			}
			
			players = _mgrs->game_manager.getNumPlayers();
			NetManager::PLAYERDATA* HostDataOut = new NetManager::PLAYERDATA[players], *HostDataIN; // not need to init, but needed to destroy
			HostDataIN = _mgrs->net_manager.HostGetPlayerInfo();  // get info from all the clients
			for(int i = 1; i<=players; i++)
			{
				// gather all players' info
				unsigned player = _mgrs->game_manager.getPlayer(i);
					HostDataOut[i-1] = {trans[player].pos.x, trans[player].pos.y};

				if(i == 1) continue;  // Skip updating the local player
				trans[_mgrs->game_manager.getPlayer(i)].pos.x = HostDataIN[i-1].x;
				trans[_mgrs->game_manager.getPlayer(i)].pos.y = HostDataIN[i-1].y;
			}
			_mgrs->net_manager.HostSendPlayerInfo(HostDataOut);
			delete HostDataOut, HostDataIN;
		}
		else // client 
		{
			unsigned player1 = _mgrs->game_manager.getPlayer(1);

			// send local player info
			data = {trans[player1].pos.x, trans[player1].pos.y};
			_mgrs->net_manager.ClientSendPlayerInfo(data);
	
			// update remote players' positions on the local screen
			players = _mgrs->game_manager.getNumPlayers();
			for(int i = 2; i<=players; i++)
			{
				data = _mgrs->net_manager.ClientGetPlayerInfo();
					trans[_mgrs->game_manager.getPlayer(i)].pos.x = data.x;
					trans[_mgrs->game_manager.getPlayer(i)].pos.y = data.y;
			}		
		}
	}
}
