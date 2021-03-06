#include "systems/view_system.h"
#include "Components.h"
#include "events.h"

void ViewSystem::update(Environment* _env, sf::RenderWindow* window, float dt)
{
	auto resize_event = _env->getEvents<ResizeEvent>();
	if(resize_event.size() > 0)
	{
		sf::FloatRect viewableArea(0, 0, resize_event[0].width, resize_event[0].height);
		window->setView(sf::View(viewableArea));
	}
	auto view_controller = _env->get<ViewController>();

	for(unsigned i = 0; i < _env->maxEntities(); i++)
	{
		if(_env->hasComponents<ViewController>(i))
		{
			auto destroyed = _env->getEvents<DestroyEvent>();
			std::vector<unsigned>& focusedObjects = view_controller[i].focusedObjects;
			for(unsigned j=0;j<destroyed.size();j++)
			{
				for(unsigned k=0;k<focusedObjects.size();)
				{
					if(focusedObjects[k] == destroyed[j].entity)
						focusedObjects.erase(focusedObjects.begin() + k);
					else
						k++;
				}
			}

			auto created = _env->getEvents<CreateEvent>();
			for(unsigned j=0;j<created.size();j++)
			{
				if(_env->hasComponents<Player>(created[j].entity))
					focusedObjects.push_back(created[j].entity);
			}


			if(focusedObjects.size() == 0)
			{
				std::cout<<"no focused objects!"<<std::endl;
				continue;
			}
			
			float ratio = (float)window->getSize().x/(float)window->getSize().y;
			ratio *= view_controller[i].viewport.width/view_controller[i].viewport.height;
			float prev_ratio = view_controller[i].prevRatio;

			sf::Vector2f viewSize, viewCenter;
			float margin = view_controller[i].margin;
			float minWidth = view_controller[i].minWidth;
			float maxWidth = view_controller[i].maxWidth;
			sf::FloatRect& borders = view_controller[i].borders;
			sf::Vector2f& prevViewSize = view_controller[i].prevViewSize;
			sf::Vector2f& prevViewCenter = view_controller[i].prevViewCenter;
			
			getMinimalView(_env, viewCenter, viewSize, focusedObjects);

			if(focusedObjects.size() == 1)
			{
				viewSize = sf::Vector2f(minWidth, minWidth / ratio);
			}

			else // more than one focused objects
			{
				correctAspectRatio(viewSize, margin, ratio);
				viewSizeInRange(viewSize, minWidth, maxWidth, ratio);	
			}

			viewInBorders(viewCenter, viewSize, borders, ratio);
			smoothMovement(
				viewCenter, viewSize, dt, 
				prevViewSize, prevViewCenter, ratio == prev_ratio);
			
			// set the new view coordinates and size
			/*
			view_controller[i].view.setCenter(viewCenter);
			view_controller[i].view.setSize(viewSize);
			view_controller[i].prevRatio = ratio;
			*/
			view_controller[i].view.setCenter(sf::Vector2f(500.f, 500.f));
			view_controller[i].view.setSize(sf::Vector2f(2048.f, 1440.f));

		}
	}
}

void ViewSystem::getMinimalView(
	Environment* _env, sf::Vector2f& _viewCenter, sf::Vector2f& _viewSize,
	std::vector<unsigned>& focusedObjects)
{
	auto transform = _env->get<Transform>();

	sf::Vector2f min = transform[focusedObjects[0]].pos.sfmlVector<float>();
	sf::Vector2f max = min;

	// compute the "average" point
	_viewCenter = sf::Vector2f(0.f, 0.f);
	for(size_t i=0;i<focusedObjects.size();i++)
	{
		const sf::Vector2f pos = transform[focusedObjects[i]].pos.sfmlVector<float>();
		_viewCenter.x += pos.x;
		_viewCenter.y += pos.y;

		if(min.x > pos.x) min.x = pos.x;
		if(min.y > pos.y) min.y = pos.y;
		if(max.x < pos.x) max.x = pos.x;
		if(max.y < pos.y) max.y = pos.y;
	}

	_viewCenter.x /= (float)focusedObjects.size();
	_viewCenter.y /= (float)focusedObjects.size();

	_viewSize.x = max.x - min.x; 
	_viewSize.y = max.y - min.y;
}

void ViewSystem::correctAspectRatio(sf::Vector2f& _viewSize, float _margin, float _ratio)
{
	// sides of viewSize should be at least one
	_viewSize.x = _viewSize.x == 0.f ? 1.f : _viewSize.x;
	_viewSize.y = _viewSize.y == 0.f ? 1.f : _viewSize.y;

	// scale a bit to add margin
	_viewSize.x *= (1.f + _margin);
	_viewSize.y *= (1.f + _margin);

	float currRatio = _viewSize.x / _viewSize.y;

	// correct width or height to have the
	// same ratio as the viewport
	if(currRatio > _ratio)
		_viewSize.y = _viewSize.x / _ratio;
	else
		_viewSize.x = _viewSize.y * _ratio;
	
	
}

void ViewSystem::viewSizeInRange(sf::Vector2f& _viewSize, float minWidth, float maxWidth, float ratio)
{
	// check if width is in range
	if(_viewSize.x < minWidth || _viewSize.x > maxWidth)
	{
		_viewSize.x = _viewSize.x < minWidth ? minWidth : maxWidth;
		_viewSize.y = _viewSize.x / ratio;
	}
}

void ViewSystem::viewInBorders(sf::Vector2f& _viewCenter, sf::Vector2f& _viewSize, const sf::FloatRect& borders, float ratio)
{
	if(_viewSize.x > borders.width) 
	{
		_viewSize.x = borders.width;
		_viewSize.y = _viewSize.x / ratio;
	}

	if(_viewSize.y > borders.height) 
	{
		_viewSize.y = borders.height;
		_viewSize.x = _viewSize.y * ratio;
	}

	if(_viewCenter.x - _viewSize.x/2.f < borders.left) 
		_viewCenter.x += borders.left - (_viewCenter.x - _viewSize.x/2.f);
	if(_viewCenter.x + _viewSize.x/2.f > borders.left + borders.width) 
		_viewCenter.x += borders.left + borders.width - (_viewCenter.x + _viewSize.x/2.f);
	if(_viewCenter.y - _viewSize.y/2.f < borders.top) 
		_viewCenter.y += borders.top - (_viewCenter.y - _viewSize.y/2.f);
	if(_viewCenter.y + _viewSize.y/2.f > borders.top + borders.height) 
		_viewCenter.y += borders.top + borders.height - (_viewCenter.y + _viewSize.y/2.f);
}

void ViewSystem::smoothMovement(
	sf::Vector2f& _viewCenter, sf::Vector2f& _viewSize, float _elapsedTime, 
	sf::Vector2f& prevViewSize, sf::Vector2f& prevViewCenter, bool prevInit)
{
	if(prevInit)
	{
		sf::Vector2f deltaViewSize, deltaViewCenter;
		deltaViewSize = _viewSize - prevViewSize;
		deltaViewCenter = _viewCenter - prevViewCenter;

		_viewSize = prevViewSize + deltaViewSize * (_elapsedTime/cameraCooldown);
		_viewCenter = prevViewCenter + deltaViewCenter * (_elapsedTime/cameraCooldown);
	}

	prevViewSize = _viewSize;
	prevViewCenter = _viewCenter;
}
