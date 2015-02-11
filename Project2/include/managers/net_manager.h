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
			unsigned playerNum, numPlayers;
		};

		static const bool CLIENT = 0, HOST = 1;
		unsigned LOCAL_PLAYER = 0;

		// both
		bool InitOnlineMode(bool _mode);
		void registerPlayer(unsigned _player);
		bool isHost() const;
		bool isConnected() const;
		void closeConnection();
		
		// client
		unsigned connectToHost(std::string _IP);
		bool isClientConnected2Host();
		unsigned getClientLatency() const;
		unsigned ClientGetPlayerInfo(std::vector<PLAYERDATA>& _dataIn);
		void ClientSendPlayerInfo(PLAYERDATA _dataOut);
		void setHostIP(std::string _IP);
		std::string getHostIP();

		// host
		bool checkForIncomingPlayers(Managers* _mgrs);
		void HostGetPlayerInfo(std::vector<PLAYERDATA>& _dataIn);
		void HostSendPlayerInfo(std::vector<PLAYERDATA>& _dataOut, Managers* mgr);

	private:
		enum CLIENT
		{
			SOCKET,
			LATENCY,
			RESPONCE,
			CONNECTED
		};

		const unsigned short port = 3333;
		bool mode; // 0 = client, 1 = host
		sf::IpAddress IP;
		sf::TcpListener listener;
		std::tuple<sf::TcpSocket*, int, bool, bool> client;				// used for client side
		std::vector<std::tuple<sf::TcpSocket*, int, bool, bool>> clients; // used for host side

		void LatencyUpdate(std::tuple<sf::TcpSocket*, int, bool, bool>& _client);
		bool Ping(sf::TcpSocket* _socket, unsigned& _playerNum);
};

