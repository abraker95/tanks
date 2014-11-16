#include "DEBUG.h"
#include "utils.h"
#include "Camera.h"

Camera::Camera(Vector2f _borders, Vector2f _minView, Vector2f _maxView, FloatRect _viewport)
	: View(sf::FloatRect(0, 0, 300, 300)) // default view
{
	setViewport(_viewport);
	borders = _borders;
	
	minView = _minView;
	maxView = _maxView;

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
	_objBounds.left   = MIN(objBounds.left,   _obj->getPosition().x-_obj->getSize().x/2-borders.x);
	_objBounds.top	  = MIN(objBounds.top,    _obj->getPosition().y-_obj->getSize().y/2-borders.y);
	_objBounds.width  = MAX(objBounds.width,  _obj->getPosition().x+_obj->getSize().x/2+borders.x);
	_objBounds.height = MAX(objBounds.height, _obj->getPosition().y+_obj->getSize().y/2+borders.y);
}

void Camera::Update(RenderWindow* _window)
{
	if(focusedObjects.size()==0)
	{
		PRINT_DEBUG(cout<<"[CAMERA]: No focused objects!", MED_DEBUG);
		return;
	}

	// Find the max boundaries at which objects exist
	objBounds = FloatRect(INFINITY, INFINITY, -INFINITY, -INFINITY);
	for(size_t i = 0; i<focusedObjects.size(); i++)
		UpdateObjectBounds(objBounds, focusedObjects[i]);

	// construct the rectangular bound perimeter
	float leftView   = MAX(objBounds.left,   minView.x),
		  topView    = MAX(objBounds.top,    minView.y),
		  rightView  = MIN(objBounds.width,  maxView.x),
		  bottomView = MIN(objBounds.height, maxView.y);

	// Viewsize code
	Vector2f viewSize = Vector2f(0, 0);
		/// \NOTE: While this contains the viewing field within the veiwing area limits, it ruins the aspect ratio correction
		//viewSize.x = LIMIT(2*borders.x, MAX(DELTA(leftView, rightView), DELTA(topView, bottomView)), DELTA(minView.x, maxView.x));
		//viewSize.y = LIMIT(2*borders.y, MAX(DELTA(leftView, rightView), DELTA(topView, bottomView)), DELTA(minView.y, maxView.y));

		// set the viewing area to complement a 1:1 aspect ratio while keeping all focused objects in view 
		viewSize.x = MAX(DELTA(leftView, rightView), DELTA(topView, bottomView));
		viewSize.y = MAX(DELTA(leftView, rightView), DELTA(topView, bottomView));
	
		// normalize window dimentions
		VectorLN winDim((Vector2f)_window->getSize());
		winDim = winDim.Normalize();

		// fix aspect ratio
		Vector2f newRatio = Vector2f(winDim.x*DELTA(getViewport().width, getViewport().left),
									 winDim.y*DELTA(getViewport().height, getViewport().top));
		viewSize.x *= newRatio.x;
		viewSize.y *= newRatio.y;

	// viewcenter code
	float viewCenterX, viewCenterY;
		if(maxView.x-minView.x < viewSize.x)        // if the viewing area limits are less than the viewing size
			viewCenterX = (maxView.x+minView.x)/2;  // set to the middle of the game area 
		else 
			viewCenterX = LIMIT(minView.x+viewSize.x/2, (leftView+rightView)/2, maxView.x-viewSize.x/2);

		if(maxView.y-minView.y < viewSize.y)       // if the viewing area limits are less than the viewing size
			viewCenterY = (maxView.y+minView.y)/2; // set to the middle of the game area 
		else
			viewCenterY = LIMIT(minView.y+viewSize.y/2, (topView+bottomView)/2, maxView.y-viewSize.y/2);

		Vector2f viewCenter(viewCenterX, viewCenterY);

	// Apply the smooth movement effect to camera
	/// \TODO: apply elapsetime to the devisors

	/// \TODO: These devisors have to be adjusted to follow objects better. Consider making a setMaxCameraSpeed funtion.
	deltaViewCenter = viewCenter-currViewCenter;
	currViewCenter += Vector2f(deltaViewCenter.x/200.0, deltaViewCenter.y/200.0);
	
	
	deltaViewSize = viewSize-currViewSize;
	
	// if the window size has changed, skip applying the effect to the viewsize and just set it
	if(ratio!=newRatio)
	{
		ratio = newRatio;
		currViewSize = viewSize;
	}
	else
	{
		currViewSize += Vector2f(deltaViewSize.x/500.0, deltaViewSize.y/500.0);
	}

	// update the center and size
	setCenter(currViewCenter);
	setSize(currViewSize);

	/// \NOTE: For camera debugging, use this
	//setCenter(viewCenter);
	//setSize(viewSize);

	// render
	_window->setView(*this);
	for(size_t i = 0; i<focusedObjects.size(); i++)
		_window->draw(*focusedObjects[i]);
}
