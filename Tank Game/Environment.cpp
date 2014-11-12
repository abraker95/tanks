//-------------------------------------------------------------
// Environment
//
// What is here:
//		- all the relations between objects is expressed here
//		- updates and render each objects
// What can improved:
// 		- reusability of code
//-------------------------------------------------------------
#include "Environment.h"

Environment::Environment()
{
	player1 = new Tank(); /// \TODO: game objects will be part of another class

	Tank::Inputmap imap{Keyboard::Right, Keyboard::Left, Keyboard::Up, Keyboard::Down};
	player1->setInput(imap);
	objects.push_back(player1);
}

Environment::~Environment()
{
	for(signed int i = 0; i<objects.size(); i++)
		if(objects[i] != nullptr)
			delete objects[i];
	objects.clear();
	/*for(int i=0;i<bullets.size();i++)
		delete bullets[i];
	bullets.clear();*/
	//delete player1;
}

void Environment::Loop(RenderWindow* _window)
{
    //player1->setPosition(0, 0);

	Clock clock;
	while(_window->isOpen())
	{
		Event event;
		while(_window->pollEvent(event))
		{
 			if(event.type == Event::Closed)
				_window->close();
		}
	
		Time elapsed = clock.restart();
		Update(elapsed.asSeconds());

		_window->clear();
			Render(_window);
		_window->display();
	}
}

void Environment::Update(float _elapsedTime)
{
	for(signed int i = 0; i<objects.size(); i++)
	{
		if(objects[i] != nullptr)
		{
			objects[i]->Update(_elapsedTime);
			if(objects[i]->isDestroy())
			{
				delete objects[i];
				objects.erase(objects.begin()+i);
			}
		}
		else
		{
			PRINT_DEBUG(cout<<"[ENV]: Found undeleted object!", MED_DEBUG);
			objects.erase(objects.begin() + i);
		}
	}

	// update tank ( player1 ) states
	/*player1->UpdateUserInput();
	player1->Update(_elapsedTime);
	if(Keyboard::isKeyPressed(Keyboard::Space) && player1->Fire())
		bullets.push_back(new Bullet(player1->getPosition(), player1->getRotation() + 90));

	// update bullets states
	int bulletIndex = 0;
	while(bulletIndex < bullets.size())
	{
		bullets[bulletIndex]->Update(_elapsedTime);
		if(bullets[bulletIndex]->isDead())
		{
			delete bullets[bulletIndex];
			bullets.erase(bullets.begin() + bulletIndex);
		}
		else 
			bulletIndex++;
	}*/
}

void Environment::Render(RenderWindow* _window)
{
	for(signed int i = 0; i<objects.size(); i++)
	{
		if(objects[i] != nullptr)
			_window->draw(*(objects[i]));
		else
		{
			PRINT_DEBUG(cout<<"[ENV]: Found undeleted object!", MED_DEBUG);
			objects.erase(objects.begin() + i);
		}
	}
	/*// render bullets
	for(int i=0;i<bullets.size();i++)
		_window->draw(*(bullets[i]));

	// render the tank ( player1 )
	_window->draw(*player1);*/
}
