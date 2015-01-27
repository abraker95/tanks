#pragma once
#include "core/environment.h"
#include <SFML\Network.hpp>

struct Managers;

class NetManager
{
	public:
		NetManager();
		virtual ~NetManager();

		static const bool CLIENT = 0, HOST = 1;

		bool InitOnlineMode(bool _mode);
		bool checkForIncomingPlayers(Managers* _mgrs);
		bool connectToHost(std::string _IP);
		void getPlayerInfo(Environment* _env);
		void sendPlayerInfo(Environment* _env);
		std::string getHostIP();
		bool isHost() const;
		void closeConnection();

	private:
		const unsigned short port = 3333;
		bool mode; // 0 = client, 1 = host
		sf::IpAddress IP;
		sf::TcpListener listener;
		sf::TcpSocket client;
		//std::vector<sf::TcpSocket> clients;
};

