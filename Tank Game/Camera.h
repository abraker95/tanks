#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "GameObject.h"

class Camera : public View
{
public:
	Camera(Vector2f _borders, Vector2f _minView = Vector2f(0, 0), Vector2f _maxView = Vector2f(0, 0), FloatRect _viewport = FloatRect(0.f, 0.f, 1.f, 1.f));
	virtual ~Camera();

	void addFocused(GameObject* _obj);
	void removeFocused(GameObject* _obj);
	void UpdateObjectBounds(FloatRect& objBounds, GameObject* _obj);
	void Update(RenderWindow* _window);

private:
	std::vector<GameObject*> focusedObjects;
	FloatRect objBounds;
	Vector2f minView, maxView,
			 borders;
	Vector2f ratio;

	// for a nice smooth effect
	Vector2f deltaViewSize, currViewSize,
			 deltaViewCenter, currViewCenter;
};
