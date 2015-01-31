#include "managers/net_manager.h"
#include "managers/managers.h"
#include "components.h"

NetManager::NetManager()
{}


NetManager::~NetManager()
{}

bool NetManager::InitOnlineMode(bool _mode)
{
	mode = _mode;
	if(mode==HOST)
	{
		listener.setBlocking(false);
		if(listener.listen(port)!=sf::Socket::Status::Done)
		{
			cout<<"[NET MGR] Unable to bind to port "<<port<<endl;
			return false;
		}
		cout<<"[NET MGR] Binded to port "<<port<<endl;

		clients.clear();
		clients.push_back(std::pair<sf::TcpSocket*, int>(new sf::TcpSocket, 0));
		clients.at(0).first->setBlocking(false);
		clients.at(0).second = 0;
	}
	else
	{
		client.setBlocking(false);
		listener.setBlocking(false);
	}

	return true;
}

unsigned NetManager::connectToHost(std::string _IP)
{
	if(client.connect(_IP, port, sf::Time(sf::seconds(5))) == sf::Socket::Done)  // give a 5 second timout
	{
		cout<<"[NET MGR] Connected to: "<<_IP<<endl;
		return 1;
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
	if(listener.accept(*clients.at(clients.size()-1).first)==sf::Socket::Status::Done)
	{
		cout<<"[NET MGR] Accepted Connection!"<<endl;
			clients.push_back(std::pair<sf::TcpSocket*, int>(new sf::TcpSocket, 0));
			clients.at(clients.size()-1).first->setBlocking(false);
		return true;
	}
	return false;
}

std::string NetManager::getHostIP()
{
	return IP.getPublicAddress().toString();
}

NetManager::PLAYERDATA* NetManager::HostGetPlayerInfo()
{
	PLAYERDATA *data = new PLAYERDATA[clients.size()];
	sf::Packet packet;

	for(int i = 0; i<clients.size(); i++)
	{
		client.receive(packet);
		if(!(packet>>data[i].x>>data[i].y>>data[i].numPlayers))
			clients.at(i).second++;

		if(clients.at(i).second > 50) // 50 failed attempts to get client's data
		{ /* game manger -> player leave,  client.erase() */ }
	}

	return data; // to be deleted by caller
}

NetManager::PLAYERDATA NetManager::ClientGetPlayerInfo()
{
	PLAYERDATA data;
	sf::Packet packet;
		client.receive(packet);

	if(!(packet>>data.x>>data.y>>data.numPlayers))
		cout<<"[NET MGR] Error getting data packet"<<endl;
	return data;
}

void NetManager::ClientSendPlayerInfo(PLAYERDATA _player)
{
	sf::Packet packet;
	packet<<_player.x<<_player.y<<clients.size(); // clients will have 0 as their clients size

	client.send(packet);
}

void NetManager::HostSendPlayerInfo(PLAYERDATA _players[])
{
	sf::Packet packet;

	for(int i = 0; i<clients.size(); i++)
	{
		packet<<_players[i].x<<_players[i].y<<clients.size(); // clients will have 0 as their clients size
		client.send(packet);
		packet.clear();
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
	clients.clear();
}
