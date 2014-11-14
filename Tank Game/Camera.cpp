#include "DEBUG.h"
#include "utils.h"
#include "Camera.h"

Camera::Camera(const FloatRect& _borders, const FloatRect& _viewport, float _ratio, float _minWidth, float _maxWidth)
	: View(sf::FloatRect(0, 0, 300, 300)) // default view
{
	setViewport(_viewport);
	borders = _borders;
	
	minWidth = _minWidth;
	maxWidth = _maxWidth;

	ratio = _ratio;
}

Camera::~Camera()
{
}

void Camera::addFocused(GameObject* _obj)
{
	focusedObjects.push_back(_obj);
}

void Camera::Update()
{
	if(focusedObjects.size() == 0)
	{
		PRINT_DEBUG(cout<<"[CAMERA]: No focused objects!", MED_DEBUG);
		return;
	}

	// compute the "average" point
	Vector2f viewSize(minWidth, minWidth / ratio);
	Vector2f viewCenter(0.f, 0.f);

	Vector2f min(focusedObjects[0]->getPosition().x, focusedObjects[0]->getPosition().y);
	Vector2f max = min;

	for(size_t i=0;i<focusedObjects.size();i++)
	{
		const Vector2f& pos = focusedObjects[i]->getPosition();
		viewCenter.x += pos.x;
		viewCenter.y += pos.y;

		if(min.x > pos.x) min.x = pos.x;
		if(min.y > pos.y) min.y = pos.y;
		if(max.x < pos.x) max.x = pos.x;
		if(max.y < pos.y) max.y = pos.y;
	}

	viewCenter.x /= (float)focusedObjects.size();
	viewCenter.y /= (float)focusedObjects.size();

	// if there is more than one focused object, compute the view
	// size with the correct ratio
	Vector2f neededView(max.x - min.x, max.y - min.y);
	if(neededView.x != 0.f || neededView.y != 0.f)
	{
		if(neededView.x == 0.f) neededView.x = 1.f;
		if(neededView.y == 0.f) neededView.y = 1.f;

		// scale a bit to add margin
		neededView.x *= 1.5f;
		neededView.y *= 1.5f;

		float currRatio = neededView.x / neededView.y;

		// correct width or height to have the
		// same ratio as the viewport
		if(currRatio > ratio)
			neededView.y = neededView.x / ratio;
		else
			neededView.x = neededView.y * ratio;
		
		// check if width is in range
		if(neededView.x < minWidth || neededView.x > maxWidth)
		{
			neededView.x = neededView.x < minWidth ? minWidth : maxWidth;
			neededView.y = neededView.x / ratio;
		}

		viewSize = neededView;
	}

	// borders correction
	if(viewSize.x > borders.width) 
	{
		viewSize.x = borders.width;
		viewSize.y = viewSize.x / ratio;
	}

	if(viewSize.y > borders.height) 
	{
		viewSize.y = borders.height;
		viewSize.x = viewSize.y * ratio;
	}

	if(viewCenter.x - viewSize.x/2.f < borders.left) 
		viewCenter.x += borders.left - (viewCenter.x - viewSize.x/2.f);
	if(viewCenter.x + viewSize.x/2.f > borders.left + borders.width) 
		viewCenter.x += borders.left + borders.width - (viewCenter.x + viewSize.x/2.f);
	if(viewCenter.y - viewSize.y/2.f < borders.top) 
		viewCenter.y += borders.top - (viewCenter.y - viewSize.y/2.f);
	if(viewCenter.y + viewSize.y/2.f > borders.top + borders.height) 
		viewCenter.y += borders.top + borders.height - (viewCenter.y + viewSize.y/2.f);
	

	// set the new view coordinates and size
	setCenter(viewCenter);
	setSize(viewSize);
}
