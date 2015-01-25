#pragma once
#include "core/environment.h"
#include "managers/managers.h"

class NetworkSystem: public System
{
	public:
		NetworkSystem();
		~NetworkSystem();

		void update(Environment* _mainEnv, Managers* _mgrs);

		unsigned checkForIncomingPlayers();
		/// getPlayerInfo();
		/// sendPlayerInfo();

	private:
		unsigned hostIP;
		vector<unsigned> clientIP;
};

