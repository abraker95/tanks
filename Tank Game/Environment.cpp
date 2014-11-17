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
	cameras.push_back(new Camera(Vector2f(200, 200), Vector2f(0, 0), Vector2f(1275, 700), FloatRect(0.f, 0.f, 1.0f, .5f)));
}

Environment::~Environment()
{
	for(size_t i = 0; i<objects.size(); i++)
		if(objects[i] != nullptr)
			delete objects[i];
	objects.clear();

	for(size_t i = 0; i<cameras.size(); i++)
		if(cameras[i]!=nullptr)
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

void Environment::addObject(GameObject* _obj, bool _focused, int _camera)
{ 
 objects.push_back(_obj); 

 if(_focused == true)
	 if(cameras.size() > _camera)
		 if(cameras[_camera]!=nullptr)
			 cameras[_camera]->addFocused(_obj);
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
				// remove object from view
				for(size_t j = 0; j<cameras.size(); j++)
					cameras[j]->removeFocused(objects[i]);

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
{	// Update cameras
	for(size_t i = 0; i<cameras.size(); i++)
		cameras[0]->Update(_window);

	// render the tilemap
	tilemap->setScale(1.f, 1.f);
	_window->draw(*tilemap);

	// render the game objects
	for(size_t i = 0; i<objects.size(); i++)
	{
		if(objects[i] != nullptr)
		{
			if(objects[i]->camNumFocus==-1) // render objects that aren't being focused
			{
				_window->draw(*objects[i]);
			}
			//objects[i]->isCollision(objects[i]);  // for debugging purposes
		}	
		else
		{
			PRINT_DEBUG(cout<<"[ENV]: Found deleted object in object pool!", MED_DEBUG);
			objects.erase(objects.begin() + i);
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

