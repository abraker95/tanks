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
//#include "GameScene.h"

Environment* Environment::singleton = nullptr;

const std::vector<GameObject*>& Environment::getObjects() const
{
	return gameScene->objects;
}

Environment::Environment()
{ 
	gameScene = new GameScene();
}

Environment::~Environment()
{
	delete gameScene;
}

void Environment::initSingleton()
{
	singleton = new Environment;
}

void Environment::deinitSingleton()
{
	if(singleton) delete singleton;
}

void Environment::addObject(GameObject* _obj)
{ 
	gameScene->objects.push_back(_obj); 
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
			if(event.type == Event::Resized)
			{
				float newRatio = (float)_window->getSize().x/(float)_window->getSize().y;
				for(size_t i=0;i<gameScene->cameras.size();i++)
					gameScene->cameras[i]->setRatio(newRatio);

			}
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
	EventMsgr::Event_Type type = EventMsgr::Event_Type::OBJ_CREATE;
	EventMsgr::AbstVar<GameObject*>* obj = nullptr;

	// add objects that need to be added
	do
	{
		obj = event.getEvent<GameObject*>(type);
		if(obj!=nullptr)
			if(obj->type == &typeid(GameObject))
				addObject((GameObject*)obj->var);
	} while(obj!=nullptr);


	for(size_t i = 0; i<gameScene->objects.size(); i++)
	{
		if(gameScene->objects[i]!=nullptr)
		{
			if(gameScene->objects[i]->isDestroy())
			{
				// remove object from view
				for(size_t j = 0; j<gameScene->cameras.size(); j++)
					gameScene->cameras[j]->removeFocused(gameScene->objects[i]);

				//PRINT_DEBUG(cout<<"Destroying object: "<<i<<endl, MED_DEBUG);
				delete gameScene->objects[i];
				gameScene->objects.erase(gameScene->objects.begin()+i);
			}
			else
			{
				gameScene->objects[i]->Update(_elapsedTime);
				checkCollisions(gameScene->objects[i]);
			}
		}
		else
		{
			PRINT_DEBUG(cout<<"[ENV]: Found deleted object in object pool!", MED_DEBUG);
			gameScene->objects.erase(gameScene->objects.begin()+i);
		}
	}

	// Update the cameras framing
	for(size_t i = 0; i<gameScene->cameras.size(); i++)
		gameScene->cameras[i]->Update(_elapsedTime);
}

void Environment::Render(RenderWindow* _window)
{	
	for(size_t i = 0; i<gameScene->cameras.size(); i++)
	{
		_window->setView(*gameScene->cameras[i]);

		// render the tilemap
		_window->draw(gameScene->tilemap);

		// render the game objects
		for(size_t i = 0; i<gameScene->objects.size(); i++)
		{
			//bool erased = false;
			if(gameScene->objects[i] != nullptr)
			{
				_window->draw(*gameScene->objects[i]);
				//objects[i]->isCollision(objects[i]);  // for debugging purposes
			}	
			else
			{
				PRINT_DEBUG(cout<<"[ENV]: Found deleted object in object pool!", MED_DEBUG);
				gameScene->objects.erase(gameScene->objects.begin() + i);
			}
		}
	}
}

// TODO: optimize collision detection
void Environment::checkCollisions(GameObject* _obj)
{
	// May be buggy when multiple objects are colliding with obj
	for(size_t i=0; i<gameScene->objects.size(); i++)
	{
		if(_obj->isSolid() && gameScene->objects[i]->isSolid())
		{
			if(gameScene->objects[i] != _obj && _obj->isCollidingWith(gameScene->objects[i]))
			{
				_obj->collisionFeedback(gameScene->objects[i]);
			}
		}
	}
}
