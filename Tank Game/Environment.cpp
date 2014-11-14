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

Environment* Environment::singleton = nullptr;

Environment::Environment()
{
	
}

Environment::~Environment()
{
	for(size_t i = 0; i<objects.size(); i++)
		if(objects[i] != nullptr)
			delete objects[i];
	objects.clear();

	for(size_t i = 0; i<cameras.size(); i++)
		if(cameras[i] != nullptr)
			delete cameras[i];
	cameras.clear();
}

void Environment::initSingleton()
{
	singleton = new Environment;
}

void Environment::deinitSingleton()
{
	if(singleton) delete singleton;
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
	for(size_t i = 0; i<objects.size(); i++)
	{
		if(objects[i] != nullptr)
		{
			if(objects[i]->isDestroy())
			{
				//PRINT_DEBUG(cout<<"Destroying object: "<<i<<endl, MED_DEBUG);
				delete objects[i];
				objects.erase(objects.begin()+i);

			}
			else
			{
				objects[i]->Update(_elapsedTime);
				checkCollisions(objects[i]);
			}
		}
		else
		{
			PRINT_DEBUG(cout<<"[ENV]: Found deleted object in object pool!", MED_DEBUG);
			objects.erase(objects.begin() + i);
		}
	}
}

void Environment::Render(RenderWindow* _window)
{
	for(size_t i=0;i<cameras.size();i++)
	{
		cameras[i]->Update();
		_window->setView(*cameras[i]);

		// render the tilemap
		tilemap->setScale(1.f, 1.f);
		_window->draw(*tilemap);

		// render the game objects
		for(size_t i = 0; i<objects.size(); i++)
		{
			//bool erased = false;
			if(objects[i] != nullptr)
			{
				_window->draw(*objects[i]);
			}	
			else
			{
				PRINT_DEBUG(cout<<"[ENV]: Found deleted object in object pool!", MED_DEBUG);
				objects.erase(objects.begin() + i);
			}
		}
	}
}

// TODO: optimize collision detection
void Environment::checkCollisions(GameObject* _obj)
{
	// May be buggy when multiple objects are colliding with obj
	for(size_t i=0; i<objects.size(); i++)
	{
		if(_obj->isSolid() && objects[i]->isSolid())
		{
			if(objects[i] != _obj && _obj->isCollidingWith(objects[i]))
			{
				_obj->collisionFeedback(objects[i]);
			}
		}
	}
}

