#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "GameObject.h"

class Camera : public View
{
public:
	Camera(const FloatRect& _borders, const FloatRect& _viewport, float _ratio, float _minWidth, float _maxWidth, float _margin);
	virtual ~Camera();

	void addFocused(GameObject* _obj);
	void removeFocused(GameObject* _obj);
	void Update(float _elapsedTime);

	void setRatio(float _ratio);

private:
	// the minimal view square where every objects fits
	// no aspect ratio correction, no minWidth, maxWidth correction
	void getMinimalView(Vector2f& _viewCenter, Vector2f& _viewSize);

	// only use this when there is more than one focused object
	void correctAspectRatio(Vector2f& _viewSize);

	// check if the viewSize is not too big or too small ( according to minWidth and maxWidth )
	// corrects if needed
	void viewSizeInRange(Vector2f& _viewSize);

	// corrects if out of borders
	void viewInBorders(Vector2f& _viewCenter, Vector2f& _viewSize);

	// interpolate according to cameraCooldown
	void smoothMovement(Vector2f& _viewCenter, Vector2f& _viewSize, float _elapsedTime);

	std::vector<GameObject*> focusedObjects;
	float minWidth, maxWidth;
	FloatRect borders;
	FloatRect viewport;
	float ratio;
	float cameraCooldown;
	float margin;
	Vector2f prevViewSize, prevViewCenter; // previous view center and view size
	bool prevInit;
};
