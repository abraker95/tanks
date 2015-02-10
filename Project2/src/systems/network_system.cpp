#include "systems/network_system.h"
#include "Components.h"

NetworkSystem::NetworkSystem() { runEvery = 10; }


NetworkSystem::~NetworkSystem()
{}

void NetworkSystem::update(Environment* _env, Managers* _mgrs)
{
	if(_mgrs->game_manager.isOnline())
	{
		unsigned numClients;
		auto trans = _env->get<Transform>();
		
		/** \NOTE: 
				* number of clients = number of players-1  (because not counting itself)
				* Player 0 is always the local player
		*/
		numClients = _mgrs->game_manager.getNumPlayers();
		NetManager::PLAYERDATA clientData;
		std::vector<NetManager::PLAYERDATA> dataOut, dataIn;
			dataOut.resize(numClients);
			dataIn.resize(numClients);

		if(_mgrs->net_manager.isHost())
		{
			if(_mgrs->net_manager.checkForIncomingPlayers(_mgrs))
			{
				cout<<"A player joined the game!"<<endl;
				_mgrs->game_manager.playerJoin(_env, _mgrs, _mgrs->game_manager.getNumPlayers()+1, false);
			}
			
			_mgrs->net_manager.HostGetPlayerInfo(dataIn);  // get info from all the clients -> dataIn
			for(int i = 0; i<numClients; i++)
			{
				// gather all players' info
				unsigned tank = _mgrs->game_manager.getPlayer(i+1),
						 playerNum = _mgrs->game_manager.getPlayerNum(i+1);
				dataOut.at(i) = {trans[tank].pos.x, trans[tank].pos.y, playerNum, numClients};  // player # as host has it
				
				// update players' info
				if(dataIn[i].playerNum !=0 ) // it will be 0 if no other player is connected
				{
					if(dataIn[i].playerNum == _mgrs->game_manager.getPlayerNum(NetManager::LOCAL_PLAYER)) continue;
					trans[tank].pos.x = dataIn[i].x;
					trans[tank].pos.y = dataIn[i].y;
				}
			}
			_mgrs->net_manager.HostSendPlayerInfo(dataOut, _mgrs);
		}
		else // client 
		{
			unsigned clientPlayer = _mgrs->net_manager.ClientGetPlayerInfo(dataIn);  // this will be 0 if connection exists or failed

			if(numClients>0)
			{
				if(_mgrs->net_manager.isClientConnected2Host())
				{
					for(int i = 0; i<numClients; i++) // update remote players' positions on the local screen
					{
						unsigned tank = _mgrs->game_manager.getPlayer(i+1);

						if(dataIn[i].playerNum==_mgrs->game_manager.getPlayerNum(NetManager::LOCAL_PLAYER)) continue;
						trans[tank].pos.x = dataIn[i].x;
						trans[tank].pos.y = dataIn[i].y;
					}

					unsigned tank = _mgrs->game_manager.getPlayer(NetManager::LOCAL_PLAYER);
					if(tank!=0) // if the player exists
					{
						// send local player info
						clientData = {trans[tank].pos.x, trans[tank].pos.y, _mgrs->game_manager.getPlayerNum(NetManager::LOCAL_PLAYER)};
						_mgrs->net_manager.ClientSendPlayerInfo(clientData);
					}
				}
			}
			else // not connected to host
			{
				if(clientPlayer!=0) // if host hasnt left before client joins
				{
					//_mgrs->game_manager.NewNetGame(_env, _mgrs);
					for(int i = 0; i<clientPlayer; i++)
						_mgrs->game_manager.playerJoin(_env, _mgrs, i, (i==clientPlayer-1)); // Create players and map the playerNums
				}
				else
				{
					_mgrs->net_manager.connectToHost(_mgrs->net_manager.getHostIP()); // resend connect request because we are not getting info
				}
				if(_mgrs->net_manager.getClientLatency()>20)
				{
					cout<<"[NET SYS] Connecting to host failed! Attempts: "<<10<<endl;
					_mgrs->game_manager.EndGame(_env, true);
				}
			}
		}
	}
}
