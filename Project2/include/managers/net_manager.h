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
			int playerNum, numPlayers;
		};

		static const bool CLIENT = 0, HOST = 1;
		static const unsigned LOCAL_PLAYER = 1;

		bool InitOnlineMode(bool _mode);
		bool checkForIncomingPlayers(Managers* _mgrs);
		unsigned connectToHost(std::string _IP);
		
		void ClientGetPlayerInfo(std::vector<PLAYERDATA>& _dataIn);
		void HostGetPlayerInfo(std::vector<PLAYERDATA>& _dataIn);
		void ClientSendPlayerInfo(PLAYERDATA _dataOut);
		void HostSendPlayerInfo(std::vector<PLAYERDATA>& _dataOut);

		std::string getHostIP();
		bool isHost() const;
		void closeConnection();

	private:
		enum CLIENT
		{
			SOCKET,
			LATENCY,
			RESPONCE
		};

		const unsigned short port = 3333;
		bool mode; // 0 = client, 1 = host
		sf::IpAddress IP;
		sf::TcpListener listener;
		std::tuple<sf::TcpSocket*, int, bool> client;				// used for client side
		std::vector<std::tuple<sf::TcpSocket*, int, bool>> clients; // used for host side

		void LatencyUpdate(std::tuple<sf::TcpSocket*, int, bool>& _client);
		bool ClientResponds(sf::TcpSocket* _socket, unsigned& _playerNum);
		void Ping(sf::TcpSocket* _socket, unsigned _playerNum);
};

