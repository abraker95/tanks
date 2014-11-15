#include "DEBUG.h"
#include "utils.h"
#include "Camera.h"

Camera::Camera(Vector2f _borders, Vector2f _minView, Vector2f _maxView, FloatRect _viewport, float _ratio)
	: View(sf::FloatRect(0, 0, 300, 300)) // default view
{
	setViewport(_viewport);
	borders = _borders;
	
	minView = _minView;
	maxView = _maxView;

	ratio = _ratio;

	deltaViewSize = Vector2f(0, 0);
	currViewSize = Vector2f(0, 0);
	deltaViewCenter = Vector2f(0, 0);
	currViewCenter = Vector2f(0, 0);
}

Camera::~Camera()
{
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

void Camera::UpdateObjectBounds(FloatRect& _objBounds, GameObject* _obj)
{
	// taking account the borders
	_objBounds.left   = MIN(objBounds.left,   _obj->getPosition().x-borders.x);
	_objBounds.top    = MIN(objBounds.top,    _obj->getPosition().y-borders.y);
	_objBounds.width  = MAX(objBounds.width,  _obj->getPosition().x+_obj->getSize().x+borders.x);
	_objBounds.height = MAX(objBounds.height, _obj->getPosition().y+_obj->getSize().y+borders.y);
}

void Camera::Update(RenderWindow* _window)
{
	if(focusedObjects.size()==0)
	{
		PRINT_DEBUG(cout<<"[CAMERA]: No focused objects!", MED_DEBUG);
		return;
	}

	// Find the max boundaries at which objects exist
	objBounds = FloatRect(0, 0, 0, 0);
	for(size_t i = 0; i<focusedObjects.size(); i++)
		UpdateObjectBounds(objBounds, focusedObjects[i]);
	
	// apply the boundaries to the View size
	Vector2f viewSize(MAX(objBounds.width-objBounds.left, objBounds.height-objBounds.top),
		              MAX(objBounds.width-objBounds.left, objBounds.height-objBounds.top));

	// If the minview = maxView, then zoom can be indefinite, otherwise apply the limits
	if(minView.x!=maxView.x)
	{
		if(!BTWN(minView.x, viewSize.x, maxView.x))
		{
			if(viewSize.x<minView.x) viewSize.x = minView.x;
			if(viewSize.x>maxView.x) viewSize.x = maxView.x;
		}
	}

	if(minView.y!=maxView.y)
	{
		if(!BTWN(minView.y, viewSize.y, maxView.y))
		{
			if(viewSize.y<minView.y) viewSize.y = minView.y;
			if(viewSize.y>maxView.y) viewSize.y = maxView.y;
		}
	}

	/// \TODO: apply elapsetime to the devisors
	deltaViewSize = viewSize-currViewSize;
	currViewSize += Vector2f(deltaViewSize.x/1000.0, deltaViewSize.y/1000.0);

	// View center code
	Vector2f viewCenter((objBounds.width+objBounds.left-50)/2, (objBounds.height+objBounds.top-50)/2); // basically the midpoint of the viewSize
	deltaViewCenter = viewCenter-currViewCenter;
	currViewCenter += Vector2f(deltaViewCenter.x/1000.0, deltaViewCenter.y/1000.0);

	// update the center and size
	setCenter(currViewCenter);
	setSize(currViewSize);

	// render
	_window->setView(*this);
	for(size_t i = 0; i<focusedObjects.size(); i++)
		_window->draw(*focusedObjects[i]);
}
