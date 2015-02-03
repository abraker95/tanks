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
		auto trans = _env->get<Transform>();
		
		/** \NOTE: 
				* number of clients = number of players-1  (because not counting itself)
				* Player 0 is always the local player
		*/
		players = _mgrs->game_manager.getNumPlayers();
		NetManager::PLAYERDATA clientData;
		std::vector<NetManager::PLAYERDATA> dataOut, dataIn;
			dataOut.resize(players);

		if(_mgrs->net_manager.isHost())
		{
			if(_mgrs->net_manager.checkForIncomingPlayers(_mgrs))
			{
				cout<<"A player joined the game!"<<endl;
				_mgrs->game_manager.playerJoin(_env, _mgrs, _mgrs->game_manager.getNumPlayers()+1);
				
				/// \TODO: Broadcast new player to clients
			}
			
			_mgrs->net_manager.HostGetPlayerInfo(dataIn);  // get info from all the clients -> dataIn
			for(int i = 1; i<=players; i++)
			{
				// gather all players' info
				unsigned player = _mgrs->game_manager.getPlayer(i),
						 playerNum = _mgrs->game_manager.getPlayerNum(i);
					dataOut.at(i-1) = {trans[player].pos.x, trans[player].pos.y, playerNum, players};  // player # as host has it
				
				// update players' info
				//if(i == 1) continue;  // Skip updating the local player (Host)
				if(dataIn[i-1].playerNum!=0) // it will be 0 if no other player is connected
				{
					if(dataIn[i-1].playerNum==_mgrs->game_manager.getPlayerNum(NetManager::LOCAL_PLAYER)) continue;
					trans[_mgrs->game_manager.getPlayer(i)].pos.x = dataIn[i-1].x;
					trans[_mgrs->game_manager.getPlayer(i)].pos.y = dataIn[i-1].y;
				}
			}
			_mgrs->net_manager.HostSendPlayerInfo(dataOut);
		}
		else // client 
		{
			unsigned clientPlayer = _mgrs->game_manager.getPlayer(NetManager::LOCAL_PLAYER);

			// send local player info
			clientData = {trans[clientPlayer].pos.x, trans[clientPlayer].pos.y, _mgrs->game_manager.getPlayerNum(NetManager::LOCAL_PLAYER), 1};
			_mgrs->net_manager.ClientSendPlayerInfo(clientData);
	
			// update remote players' positions on the local screen
			players = _mgrs->game_manager.getNumPlayers();
			_mgrs->net_manager.ClientGetPlayerInfo(dataIn);
			for(int i = 1; i<=players; i++)
			{
				if(dataIn[i-1].playerNum == _mgrs->game_manager.getPlayerNum(NetManager::LOCAL_PLAYER)) continue;
				trans[_mgrs->game_manager.getPlayer(i)].pos.x = dataIn[i-1].x;
				trans[_mgrs->game_manager.getPlayer(i)].pos.y = dataIn[i-1].y;
			}		
		}
	}
}
