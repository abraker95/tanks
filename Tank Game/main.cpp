#include <SFML/Graphics.hpp>
#include "Tank.h"
#include "Mine.h"
#include "Camera.h"
#include "Tilemap.h"
#include "Environment.h"

using namespace sf;

// Let this be global for now, it's here for debugging objects. 
// Unless you can come up with a way to draw graphical debugs another way.
RenderWindow* window; 

int main()
{
	
	window = new RenderWindow(VideoMode(1080, 420), "Tank Game");

	Environment::initSingleton();
	Environment* env = Environment::getSingleton();

	//--------------------------------------------------------
	// Tilemap
	//--------------------------------------------------------
	const int map[] =
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 2, 1, 1, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 1, 1, 1, 1, 0, 0, 4, 1, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 1, 1, 1, 5, 0, 0, 0, 4, 1, 1, 1, 3, 0, 2, 1, 1, 1, 1,
		0, 0, 1, 1, 5, 0, 0, 0, 0, 0, 0, 4, 1, 1, 1, 1, 1, 1, 1, 1,
		0, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 4, 1, 1, 1, 5, 0, 0, 0,
		2, 1, 1, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		1, 1, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		1, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	};

	Tilemap tilemap;
	if(!tilemap.load("Tilesheet.png", sf::Vector2u(64, 64), map, 20, 12))
	{
     	PRINT_DEBUG(cout<<"Could not load Tilesheet.png"<<endl, HI_DEBUG);
		return -1;
	}

	env->setTilemap(&tilemap);

	//--------------------------------------------------------
	// Tanks
	//--------------------------------------------------------
	Tank* player1, *player2;

	player1 = new Tank; /// \TODO: game objects will be part of another class
		Tank::Inputmap imapPlayer1(new Keyboard::Key[5]{Keyboard::Right, Keyboard::Left, Keyboard::Up, Keyboard::Down, Keyboard::Space}, {});
		player1->setInput(imapPlayer1);
		player1->setPosition(200, 500);

	player2 = new Tank;
		Tank::Inputmap imapPlayer2(new Keyboard::Key[5] { Keyboard::D, Keyboard::A, Keyboard::W, Keyboard::S, Keyboard::F }, {});

		player2->setInput(imapPlayer2);
		player2->setPosition(200, 100);

	env->addObject(player1, true);
	env->addObject(player2, true);

	//--------------------------------------------------------
	// Views
	//--------------------------------------------------------
	//sf::FloatRect borders = sf::FloatRect(0.f, 0.f, 64.f * 20.f,  64.f * 12.f);
	//sf::FloatRect viewport = sf::FloatRect(0.f, 0.f, 1.f, 1.f);
	//float ratio = (float)window->getSize().x/(float)window->getSize().y;
	//Camera cam(borders, viewport, ratio, 800.f, 1200.f);

	//cam.addFocused(player1);
	//cam.addFocused(player2);

	//env->addCamera(&cam);

	env->Loop(window);

	Environment::deinitSingleton();
	delete window;

	return 0;
}
