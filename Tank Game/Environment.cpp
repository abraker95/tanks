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
	objBounds.left = 0;
	objBounds.width = 0;
	objBounds.top = 0;
	objBounds.height = 0; 
}

Environment::~Environment()
{
	for(size_t i = 0; i<objects.size(); i++)
		if(objects[i] != nullptr)
			delete objects[i];
	objects.clear();
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

void Environment::UpdateObjectBounds(FloatRect& objBounds, GameObject* _obj)
{
	objBounds.left   = MIN(objBounds.left,  _obj->getPosition().x);
	objBounds.top    = MIN(objBounds.top,   _obj->getPosition().y);
	objBounds.width  = MAX(objBounds.width, _obj->getPosition().x+2*_obj->getBoundingCircleRadius());
	objBounds.height = MAX(objBounds.height,  _obj->getPosition().y+2*_obj->getBoundingCircleRadius());
}

void Environment::Update(float _elapsedTime)
{
	FloatRect newObjBounds(0, 0, 0, 0);

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
				UpdateObjectBounds(newObjBounds, objects[i]);

				/// Here Debugging purposes
				checkCollisions(objects[i]);
			}
		}
		else
		{
			PRINT_DEBUG(cout<<"[ENV]: Found deleted object in object pool!", MED_DEBUG);
			objects.erase(objects.begin() + i);
		}
	}

	objBounds = newObjBounds;
}

void Environment::Render(RenderWindow* _window)
{
	// Prepare camera
	_window->setView(view);
	view.setViewport(FloatRect(0.f, 0.f, 1.f, 1.f));
	view.setCenter((objBounds.width+objBounds.left-50)/2, (objBounds.height+objBounds.top-50)/2);
	view.setSize(MAX(objBounds.width-objBounds.left, objBounds.height-objBounds.top)+50, MAX(objBounds.width-objBounds.left, objBounds.height-objBounds.top)+50);

	// render the tilemap
	tilemap->setScale(1.f, 1.f);
	_window->draw(*tilemap);

	// render the game objects
	for(size_t i = 0; i<objects.size(); i++)
	{
		if(objects[i] != nullptr)
		{
			
			_window->draw(*objects[i]);
			objects[i]->isCollision(objects[i]);
		}	
		else
		{
			PRINT_DEBUG(cout<<"[ENV]: Found deleted object in object pool!", MED_DEBUG);
			objects.erase(objects.begin() + i);
		}
	}
	_window->setView(_window->getDefaultView()); // return to previous camera
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

