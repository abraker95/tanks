#include "managers/net_manager.h"
#include "managers/managers.h"
#include "components.h"

NetManager::NetManager()
{}


NetManager::~NetManager()
{
}

bool NetManager::InitOnlineMode(bool _mode)
{
	mode = _mode;
	clients.clear(); // clear regardless

	if(mode == HOST)
	{
		listener.setBlocking(false);
		if(listener.listen(port)!=sf::Socket::Status::Done)
		{
			cout<<"[NET MGR] Unable to bind to port "<<port<<endl;
			return false;
		}
		cout<<"[NET MGR] Binded to port "<<port<<endl;	

		clients.push_back(std::tuple<sf::TcpSocket*, int, bool>(new sf::TcpSocket(), 0, false));
		std::get<SOCKET>(clients.at(0))->setBlocking(false);
		std::get<SOCKET>(client) = nullptr; // not using client side "client"
	}
	else
	{
		client = std::tuple<sf::TcpSocket*, int, bool>(new sf::TcpSocket(), 0, false);
		std::get<SOCKET>(client)->setBlocking(false);
		listener.setBlocking(false);
		// server side "client" size should be 0 due to clear;
	}

	return true;
}


void NetManager::Ping(sf::TcpSocket* _socket, unsigned _playerNum)
{
	sf::Packet packet;
	std::string msg = "Ping";
	packet<<msg<<_playerNum;
	sf::Socket::Status status = _socket->send(packet);
		cout<<"Send Status: "<<status<<endl;
}

bool NetManager::ClientResponds(sf::TcpSocket* _socket, unsigned& _playerNum)
{
	sf::Packet packet;
	std::string msg;
	sf::Socket::Status status = _socket->receive(packet);
		cout<<"Recieve Status: "<<status<<endl;
	if(status == sf::Socket::Status::Done)
	{
		if(packet>>msg>>_playerNum)
		{
			cout<<"Message decoded"<<endl;
			if(msg=="Ping")
				return true;
		}
		else return false;
	}
	else return false;
}

unsigned NetManager::connectToHost(std::string _IP)
{	
	sf::TcpSocket sock;
	unsigned playerNum = 0;
	if(std::get<SOCKET>(client)->connect(_IP, port, sf::Time(sf::seconds(5))) != sf::Socket::Done)  // give a 5 second timout
	{
		cout<<"[NET MGR] Connected to: "<<_IP<<endl;
		
		while((std::get<LATENCY>(client) < 10) && !std::get<RESPONCE>(client))  // 10 tries to establish connection [client side]
		{
			Ping(std::get<SOCKET>(client), 0);
			std::get<RESPONCE>(client) = ClientResponds(std::get<SOCKET>(client), playerNum); // playerNum to be mapped
			LatencyUpdate(client);
		}

		if(std::get<RESPONCE>(client) == true)
		{
			return playerNum;
		}
		else
		{
			cout<<"[NET MGR] Ping timeout. Error connecting to host"<<endl;
			return 0;
		}
	}
	else
	{
		cout<<"[NET MGR] Error connecting to host"<<endl;
		return 0;
	}
}

bool NetManager::checkForIncomingPlayers(Managers* _mgrs)
{
	// if there is an incoming player, accept him and create a new socket for more players
	if(listener.accept(*std::get<SOCKET>(clients.at(clients.size()-1))) == sf::Socket::Status::Done)
	{
		cout<<"[NET MGR] Accepted Connection!"<<endl;
		unsigned playerNum = 0;

		while((std::get<LATENCY>(clients.at(clients.size()-1)) < 10) && !std::get<RESPONCE>(clients.at(clients.size()-1)))  // 10 tries to establish connection [server side]
		{
			//Ping(std::get<SOCKET>(clients.at(clients.size()-1)), _mgrs->game_manager.getNumPlayers()+1); // playerNum to send to client
			std::get<RESPONCE>(clients.at(clients.size()-1)) = ClientResponds(std::get<SOCKET>(clients.at(clients.size()-1)), playerNum); // playerNum is stub
			if(!std::get<RESPONCE>(clients.at(clients.size()-1))) std::get<LATENCY>(clients.at(clients.size()-1))++;
		}
		LatencyUpdate(clients.at(clients.size()-1));

		if(std::get<RESPONCE>(clients.at(clients.size()-1))==true)
		{
			clients.push_back(std::tuple<sf::TcpSocket*, int, bool>(new sf::TcpSocket(), 0, false));
			std::get<SOCKET>(clients.at(clients.size()-1))->setBlocking(false);
			return true;
		}
		else
		{
			cout<<"[NET MGR] Client didn't respond in time."<<endl;
			return false;
		}
	}
	else return false;
}

std::string NetManager::getHostIP()
{
	return IP.getPublicAddress().toString();
}

void NetManager::ClientGetPlayerInfo(std::vector<PLAYERDATA>& _dataIn)
{
	sf::Packet packet;
	float x, y;
	int playerNum, numPlayers; 

	// i = 0   get initial packet
	std::get<SOCKET>(client)->receive(packet);
	std::get<RESPONCE>(client) = (packet>>x>>y>>playerNum>>numPlayers);
	LatencyUpdate(client);

	// if(numPlayers != _mgr->GameManager(),getNumPlayers()) player join

	if(std::get<RESPONCE>(client)) // if host responded
	{
		_dataIn.resize(numPlayers);
		_dataIn[0].x = x;		_dataIn[0].y = y;
		for(int i = 1; i<_dataIn.size(); i++)
		{
			std::get<SOCKET>(client)->receive(packet);
			std::get<RESPONCE>(client) = (packet>>_dataIn[i].x>>_dataIn[i].y>>_dataIn[i].playerNum>>_dataIn[i].numPlayers);
			LatencyUpdate(client);
		}
	}
}

void NetManager::ClientSendPlayerInfo(PLAYERDATA _dataOut)
{
	sf::Packet packet;
	packet<<_dataOut.x<<_dataOut.y<<_dataOut.playerNum<<clients.size(); // clients will have 0 as their clients size
	std::get<SOCKET>(client)->send(packet);
}

void NetManager::HostGetPlayerInfo(std::vector<PLAYERDATA>& _dataIn)
{
	sf::Packet packet;
	_dataIn.resize(clients.size());
	for(int i = 0; i<clients.size(); i++)
	{
		std::get<SOCKET>(clients.at(i))->receive(packet);
		std::get<RESPONCE>(clients.at(i)) = (packet>>_dataIn[i].x>>_dataIn[i].y>>_dataIn[i].playerNum>>_dataIn[i].numPlayers);
		LatencyUpdate(clients.at(i));
	}
}

void NetManager::HostSendPlayerInfo(std::vector<PLAYERDATA>& _dataOut)
{
	sf::Packet packet;

	for(int i = 0; i<clients.size(); i++) // send each client the info about other clients
	{
		if(std::get<RESPONCE>(clients.at(i)))
		{
			for(int client = 0; client<_dataOut.size(); client++)  // send client i info about other clients
			{
				//if(i == client) continue; // skip sending client its own info
				packet<<_dataOut[i].x<<_dataOut[i].y<<_dataOut[i].playerNum<<clients.size(); // clients will have 0 as their clients size
				std::get<SOCKET>(clients.at(i))->send(packet);
				packet.clear();
			}
		}
	}
}

void NetManager::LatencyUpdate(std::tuple<sf::TcpSocket*, int, bool>& _client)
{
	if(std::get<RESPONCE>(_client) == false)
		std::get<LATENCY>(_client)++;
	else
		std::get<LATENCY>(_client) = 0;

	if(std::get<LATENCY>(_client) > 50) // 50 failed attempts to get client's data
	{
		//cout<<"Player not responding."<<endl;
		/* game manger -> player leave,  client.erase() */
	}
}

bool NetManager::isHost() const
{
	return mode;
}

void NetManager::closeConnection()
{
	cout<<"[NET MGR] Closing connection"<<endl;
	listener.close();

	if(isHost())
	{
		for(int i = 0; i<clients.size(); i++)
			std::get<SOCKET>(clients.at(i))->disconnect();
	}
	else
		std::get<SOCKET>(client)->disconnect();

	if(std::get<SOCKET>(client)!=nullptr)
		delete std::get<SOCKET>(client);
	clients.clear();
}
