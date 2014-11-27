#include "DEBUG.h"
#include "utils.h"
#include "Camera.h"

Camera::Camera(const FloatRect& _borders, const FloatRect& _viewport, float _ratio, float _minWidth, float _maxWidth, float _margin)
	: View(sf::FloatRect(0, 0, 300, 300)) // default view
{
	setViewport(_viewport);
	viewport = _viewport;
	borders = _borders;
	
	minWidth = _minWidth;
	maxWidth = _maxWidth;

	ratio = _ratio * _viewport.width/_viewport.height;
	cameraCooldown = 0.2f;

	prevViewSize = Vector2f(minWidth, minWidth / ratio);
	prevViewCenter = Vector2f(0.f, 0.f);

	prevInit = false;

	margin = _margin;
}

Camera::~Camera()
{
}

void Camera::Update(float _elapsedTime)
{
	if(focusedObjects.size() == 0)
	{
		PRINT_DEBUG(cout<<"[CAMERA]: No focused objects!", MED_DEBUG);
		return;
	}

	Vector2f viewSize, viewCenter;

	getMinimalView(viewCenter, viewSize);

	if(focusedObjects.size() == 1)
	{
		viewSize = Vector2f(minWidth, minWidth / ratio);
	}

	else // more than one focused objects
	{
		correctAspectRatio(viewSize);
		viewSizeInRange(viewSize);	
	}

	viewInBorders(viewCenter, viewSize);
	smoothMovement(viewCenter, viewSize, _elapsedTime);
	
	// set the new view coordinates and size
	setCenter(viewCenter);
	setSize(viewSize);
}

void Camera::setRatio(float _ratio)
{
	ratio = _ratio * viewport.width/viewport.height;
	prevInit = false;
}

void Camera::addFocused(GameObject* _obj)
{
	focusedObjects.push_back(_obj);
}

void Camera::removeFocused(GameObject* _obj)
{
	for(size_t i=0;i<focusedObjects.size();i++)
	{
		if(focusedObjects[i] == _obj)
		{
			focusedObjects.erase(focusedObjects.begin() + i);
			break;
		}
	}
}

void Camera::getMinimalView(Vector2f& _viewCenter, Vector2f& _viewSize)
{
	Vector2f min(focusedObjects[0]->getPosition().x, focusedObjects[0]->getPosition().y);
	Vector2f max = min;

	// compute the "average" point
	_viewCenter = Vector2f(0.f, 0.f);
	for(size_t i=0;i<focusedObjects.size();i++)
	{
		const Vector2f& pos = focusedObjects[i]->getPosition();
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

void Camera::correctAspectRatio(Vector2f& _viewSize)
{
	// sides of viewSize should be at least one
	_viewSize.x = _viewSize.x == 0.f ? 1.f : _viewSize.x;
	_viewSize.y = _viewSize.y == 0.f ? 1.f : _viewSize.y;

	// scale a bit to add margin
	_viewSize.x *= (1.f + margin);
	_viewSize.y *= (1.f + margin);

	float currRatio = _viewSize.x / _viewSize.y;

	// correct width or height to have the
	// same ratio as the viewport
	if(currRatio > ratio)
		_viewSize.y = _viewSize.x / ratio;
	else
		_viewSize.x = _viewSize.y * ratio;
	
	
}

void Camera::viewSizeInRange(Vector2f& _viewSize)
{
	// check if width is in range
	if(_viewSize.x < minWidth || _viewSize.x > maxWidth)
	{
		_viewSize.x = _viewSize.x < minWidth ? minWidth : maxWidth;
		_viewSize.y = _viewSize.x / ratio;
	}
}

void Camera::viewInBorders(Vector2f& _viewCenter, Vector2f& _viewSize)
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

void Camera::smoothMovement(Vector2f& _viewCenter, Vector2f& _viewSize, float _elapsedTime)
{
	if(prevInit)
	{
		Vector2f deltaViewSize, deltaViewCenter;
		deltaViewSize = _viewSize - prevViewSize;
		deltaViewCenter = _viewCenter - prevViewCenter;

		_viewSize = prevViewSize + deltaViewSize * (_elapsedTime/cameraCooldown);
		_viewCenter = prevViewCenter + deltaViewCenter * (_elapsedTime/cameraCooldown);
	}

	else
		prevInit = true;

	prevViewSize = _viewSize;
	prevViewCenter = _viewCenter;
}


