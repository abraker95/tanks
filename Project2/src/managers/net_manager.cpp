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

		clients.push_back(std::tuple<sf::TcpSocket*, int, bool, bool>(new sf::TcpSocket(), 0, false, false));
		std::get<SOCKET>(clients.at(0))->setBlocking(false);
		std::get<SOCKET>(client) = nullptr; // not using client side "client"
	}
	else
	{
		client = std::tuple<sf::TcpSocket*, int, bool, bool>(new sf::TcpSocket(), 0, false, false);
		std::get<SOCKET>(client)->setBlocking(false);
		listener.setBlocking(false);
		// server side "client" size should be 0 due to clear;
	}

	return true;
}

/// \TODO: REtry basic
bool NetManager::Ping(sf::TcpSocket* _socket, unsigned& _playerNum)
{
	sf::Packet packet;
	std::string msg;
	sf::Socket::Status status = _socket->receive(packet);
		cout<<"Recieve Status: "<<status<<endl;
	
	if(status==sf::Socket::Status::Done)
	{
		cout<<"Recieved message!"<<endl;
		if(packet>>msg>>_playerNum)
		{
			cout<<"Message decoded"<<endl;
			if(msg=="Ping")
				return true;
		}
		else return false;
	}
	else // if we didn't recieve a valid packet, send ping request
	{
		packet.clear();
		packet<<"Ping"<<_playerNum;
		status = _socket->send(packet);
		cout<<"Send Status: "<<status<<endl;
		return false;
	}
}

unsigned NetManager::connectToHost(std::string _IP)
{	
	unsigned playerNum = 0;

	if(std::get<SOCKET>(client)->connect(_IP, port, sf::Time(sf::seconds(1))) == sf::Socket::Done)  // give a 1 second timout
	{
		cout<<"[NET MGR] Connected to: "<<_IP<<endl;
		
		if(!std::get<RESPONCE>(client))
		{
			std::get<RESPONCE>(client) = Ping(std::get<SOCKET>(client), playerNum); // numPlayers to be recieved
			LatencyUpdate(client);
		}

		if(std::get<RESPONCE>(client) == true)
		{
			std::get<CONNECTED>(client) = true;
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
	unsigned newClient = clients.size()-1;
	//unsigned numPlayers = _mgrs->game_manager.getNumPlayers()+1; // the playerNum to send is the number of players if the player joins

	if(listener.accept(*std::get<SOCKET>(clients.at(newClient))) == sf::Socket::Status::Done)
	{
		cout<<"[NET MGR] Accepted Connection!"<<endl;
	//	unsigned playerNum = 0;
		
	/*	if(/*(std::get<LATENCY>(clients.at(newClient)) < 50) && !std::get<RESPONCE>(clients.at(newClient)))  // 50 tries to establish connection [server side]
		{
			
		}*/

		//if(std::get<RESPONCE>(clients.at(newClient))==true) // if there is a new client, create a new socket for another client for it to be ready
	//	{
		//	std::get<CONNECTED>(clients.at(newClient)) = true;
			std::get<LATENCY>(clients.at(newClient)) = 1;
			clients.push_back(std::tuple<sf::TcpSocket*, int, bool, bool>(new sf::TcpSocket(), 0, false, false));
			std::get<SOCKET>(clients.at(clients.size()-1))->setBlocking(false);
			return true;
		}
		else
		{
			//cout<<"[NET MGR] Client didn't respond in time."<<endl;
			return false;
		}
	//}
	//else return false; // no new connections
}

std::string NetManager::getHostIP()
{
	return IP.getPublicAddress().toString();
}

unsigned NetManager::ClientGetPlayerInfo(std::vector<PLAYERDATA>& _dataIn)
{
	/*
		A client gets packets from the host. This contains data about ALL players. 
		The client locally updates all the players execpt itself.
	*/

	sf::Packet packet;
	unsigned playerNum = 0, numPlayers = 0;  // playerNum is a stub while numPlayers is the thing returned if the client hasnt established coms
	std::string S;

	std::get<SOCKET>(client)->receive(packet);

	// get header
	std::get<RESPONCE>(client) = (packet>>S>>numPlayers);
	LatencyUpdate(client);

	if(std::get<CONNECTED>(client) && std::get<RESPONCE>(client)) // if host responded
	{
		_dataIn.resize(numPlayers);
		// if(numPlayers != _mgr->GameManager(),getNumPlayers()) player join
		for(int i = 0; i<_dataIn.size(); i++)
		{
			std::get<RESPONCE>(client) = (packet>>_dataIn[i].x>>_dataIn[i].y>>_dataIn[i].playerNum);
			if(!std::get<RESPONCE>(client)) cout<<"[NET MGR] Corrupted packet detected"<<endl;
		}
	}
	else // if not connected, request initial packet info
	{
		std::get<RESPONCE>(client) = Ping(std::get<SOCKET>(client), playerNum); // playerNum to send to client
		if(std::get<RESPONCE>(client)) std::get<CONNECTED>(client) = true;
		//connectToHost(IP.toString());
	}
	cout<<"Player Num: "<<numPlayers<<endl;
	return numPlayers;
}

void NetManager::ClientSendPlayerInfo(PLAYERDATA _dataOut)
{
	/*
		A client sends packets to the host. This contains data about itself.
	*/
	if(std::get<CONNECTED>(client))
	{
		sf::Packet packet;
		packet<<_dataOut.x<<_dataOut.y<<_dataOut.playerNum;
		std::get<SOCKET>(client)->send(packet);
	}
}

void NetManager::HostGetPlayerInfo(std::vector<PLAYERDATA>& _dataIn)
{
	/*
		The host gets packets from every client. The host locally updates all the players execpt itself.
	*/

	sf::Packet packet;
	_dataIn.resize(clients.size());
	for(int i = 0; i<clients.size(); i++)
	{
		if(std::get<CONNECTED>(clients.at(i))) // get the info from client only if host established a connection with it
		{
			std::get<SOCKET>(clients.at(i))->receive(packet);
			std::get<RESPONCE>(clients.at(i)) = (packet>>_dataIn[i].x>>_dataIn[i].y>>_dataIn[i].playerNum); // clients dont send the number of players
			LatencyUpdate(clients.at(i));
		}
		else // make up invalid info if not connected
		{
			_dataIn[i].x = -100;
			_dataIn[i].y = -100;
			_dataIn[i].playerNum = 0;
		}
	}
}

void NetManager::HostSendPlayerInfo(std::vector<PLAYERDATA>& _dataOut, Managers* _mgrs)
{
	/*
		the host sends packets from the every client. This contains data about ALL players.
	*/

	sf::Packet packet;

	for(int i = 0; i<clients.size(); i++) // send each client the info about other clients
	{
		if(std::get<CONNECTED>(clients.at(i)))
		{
			if(std::get<RESPONCE>(clients.at(i)))
			{
				packet<<"S"<<clients.size(); // Host packet header: "S" followed by number of players

				for(int k = 0; k<_dataOut.size(); k++)  // send client i info about other client k
					packet<<_dataOut[k].x<<_dataOut[k].y<<_dataOut[k].playerNum; // clients will have 0 as their clients size

				std::get<SOCKET>(clients.at(i))->send(packet);
				packet.clear();
			}
		}
		else if(std::get<LATENCY>(clients.at(i))> 0) // if not connected, then send connection info. Client should have registered (not an empty socket)
		{
			unsigned playerNum = _mgrs->game_manager.getNumPlayers()+1; // the playerNum to send is the number of players if the player joins
			std::get<RESPONCE>(clients.at(i)) = Ping(std::get<SOCKET>(clients.at(i)), playerNum); // playerNum to send to client
			LatencyUpdate(clients.at(i));

			if(std::get<RESPONCE>(clients.at(i))) std::get<CONNECTED>(clients.at(i)) = true;
		}
	}
}

bool NetManager::isClientConnected2Host()
{
	return std::get<CONNECTED>(client);
}

unsigned NetManager::getClientLatency() const
{
	return std::get<LATENCY>(client);
}

void NetManager::LatencyUpdate(std::tuple<sf::TcpSocket*, int, bool, bool>& _client)
{
	if(std::get<RESPONCE>(_client) == false)
		std::get<LATENCY>(_client)++;
	else
		std::get<LATENCY>(_client) = 0;

	if(std::get<LATENCY>(_client) > 50) // 50 failed attempts to get client's data
	{
		if(isHost() && clients.size()>1)
		{
			unsigned numClients = clients.size();
			std::get<RESPONCE>(_client) = Ping(std::get<SOCKET>(_client), numClients);
		}
		//cout<<"Player not responding."<<endl;
		/* game manger -> player leave,  client.erase() */
	}
}

bool NetManager::isHost() const
{
	return mode==1;
}

void NetManager::setHostIP(std::string _IP)
{
	IP = sf::IpAddress(_IP);
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
