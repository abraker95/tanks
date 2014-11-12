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
	player1 = new Tank(&objects); /// \TODO: game objects will be part of another class

	Tank::Inputmap imap{Keyboard::Right, Keyboard::Left, Keyboard::Up, Keyboard::Down, Keyboard::Space};
	player1->setInput(imap);
	//objects.push_back(player1);
}

Environment::~Environment()
{
	for(signed int i = 0; i<objects.size(); i++)
		if(objects[i] != nullptr)
			delete objects[i];
	objects.clear();
}

void Environment::Loop(RenderWindow* _window)
{
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
				//PRINT_DEBUG(cout<<"Destroying object: "<<i<<endl, MED_DEBUG);
				objects.erase(objects.begin()+i);
			}
		}
		else
		{
			PRINT_DEBUG(cout<<"[ENV]: Found undeleted object!", MED_DEBUG);
			objects.erase(objects.begin() + i);
		}
	}
}

void Environment::Render(RenderWindow* _window)
{
	for(signed int i = 0; i<objects.size(); i++)
	{
		if(objects[i] != nullptr)
		{
			//PRINT_DEBUG(cout<<"i: "<<i<<"  object list size: "<<objects.size()<<"  Object: "<<objects[i]<<endl, HI_DEBUG);
			_window->draw(*objects[i]);
		}
			
		else
		{
			PRINT_DEBUG(cout<<"[ENV]: Found undeleted object!", MED_DEBUG);
			objects.erase(objects.begin() + i);
		}
	}
}
