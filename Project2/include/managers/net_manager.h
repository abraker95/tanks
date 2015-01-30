#pragma once
#include "core/environment.h"
#include <SFML\Network.hpp>

struct Managers;

class NetManager
{
	public:
		NetManager();
		virtual ~NetManager();

		struct PLAYERDATA
		{
			float x, y;
			int numPlayers;
		};

		static const bool CLIENT = 0, HOST = 1;

		bool InitOnlineMode(bool _mode);
		bool checkForIncomingPlayers(Managers* _mgrs);
		unsigned connectToHost(std::string _IP);
		
		PLAYERDATA ClientGetPlayerInfo();
		PLAYERDATA* HostGetPlayerInfo();
		void ClientSendPlayerInfo(PLAYERDATA player);
		void HostSendPlayerInfo(PLAYERDATA players[]);

		std::string getHostIP();
		bool isHost() const;
		void closeConnection();


	private:
		const unsigned short port = 3333;
		bool mode; // 0 = client, 1 = host
		sf::IpAddress IP;
		sf::TcpListener listener;
		sf::TcpSocket client;			    // used for client side
		std::vector<std::pair<sf::TcpSocket*, int>> clients; // used for host side
};

