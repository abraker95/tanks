#include "GameScene.h"

GameScene::GameScene()
{
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

	/// \TODO: Load Tilesheet_1.png
	if(!tilemap.load("Tilesheet.png", sf::Vector2u(64, 64), map, 20, 12))
	{
		PRINT_DEBUG(cout<<"Could not load Tilesheet.png"<<endl, HI_DEBUG);
		exit(-1);
	}

	setTilemap(tilemap);

	/// \TODO: Load player's position and direction from file
	Tank* player1 = new Tank; /// \TODO: game objects will be part of another class
		Tank::Inputmap imapPlayer1(new Keyboard::Key[5]{Keyboard::Right, Keyboard::Left, Keyboard::Up, Keyboard::Down, Keyboard::Space}, {});
		player1->setInput(imapPlayer1);
		player1->setPosition(200, 500);
		addObject(player1);

	Tank* player2 = new Tank;
		Tank::Inputmap imapPlayer2(new Keyboard::Key[5] { Keyboard::D, Keyboard::A, Keyboard::W, Keyboard::S, Keyboard::F }, {});
		player2->setInput(imapPlayer2);
		player2->setPosition(200, 100);
		addObject(player2);

	//--------------------------------------------------------
	// Views
	//--------------------------------------------------------
	sf::FloatRect borders = sf::FloatRect(0.f, 0.f, 64.f * 20.f, 64.f * 12.f);
	sf::FloatRect viewport = sf::FloatRect(0.f, 0.f, 1.f, 1.f);
	float ratio = 1.0;//_winDim->x/_winDim->y;
	
	Camera* cam = new Camera(borders, viewport, ratio, 400.f, 1200.f, 0.4f);
		cam->addFocused(player1);
		cam->addFocused(player2);
		addCamera(cam);
}


GameScene::~GameScene()
{
	for(size_t i = 0; i<objects.size(); i++)
		if(objects[i]!=nullptr)
			delete objects[i];
	objects.clear();

	for(size_t i = 0; i<cameras.size(); i++)
		if(cameras[i]!=nullptr)
			delete cameras[i];
	cameras.clear();
}
