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
		client.setBlocking(false);

		if(listener.listen(port)!=sf::Socket::Status::Done)
		{
			return false;
		}
			
	}
	else
	{
		client.setBlocking(true);
		listener.setBlocking(true);
	}

	return true;
}

bool NetManager::connectToHost(std::string _IP)
{
	if(client.connect(_IP, port, sf::Time(sf::seconds(5))) != sf::Socket::Done)  // give a 5 second timout
	{
		cout<<"Error connecting to host"<<endl;
		return false;
	}
	else
	{
		cout<<"Connected to: "<<_IP<<endl;

		sf::Packet packet;
		int numPlayers;

		client.receive(packet);
		if(packet>>numPlayers)
		{
			cout<<"NumPlayers: "<<numPlayers<<endl;
		}
		return true;
	}
		
}

bool NetManager::checkForIncomingPlayers(Managers* _mgrs)
{
	/// if there are incoming players, get IP addresses and add them to client IP list
	/// if there are no incoming players, return 0 (0.0.0.0)
	if(listener.accept(client)==sf::Socket::Status::Done)
	{
		cout<<"Accepted Connection!"<<endl;
		return true;
	}
	return false;
}

void NetManager::ExchangeGreeting(Managers* _mgrs)
{
	// first packet sent tells how many players there are in the game.
	sf::Packet packet;
	packet<<_mgrs->game_manager.getNumPlayers();
	client.send(packet);

	packet.clear();

}

std::string NetManager::getHostIP()
{
	return IP.getPublicAddress().toString();
}

void NetManager::getPlayerInfo(Environment* _env)
{
	sf::Packet packet;
	client.receive(packet);
	float x, y;

	if(!(packet>>x>>y))
		cout<<"Error getting data packet"<<endl;
	Vec2f tankPos(x, y);
}

void NetManager::sendPlayerInfo(Environment* _env)
{
	sf::Packet packet;
	Vec2f tankPos = _env->getComponent<Transform>(_env->getID("tank1")).pos;
	packet<<tankPos.x<<tankPos.y;

	client.send(packet);
}

bool NetManager::isHost() const
{
	return mode;
}

void NetManager::closeConnection()
{
	listener.close();
}