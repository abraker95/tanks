#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "GameObject.h"

class Camera : public View
{
public:
	Camera(const FloatRect& _borders, const FloatRect& _viewport, float _ratio, float _minWidth, float _maxWidth);
	virtual ~Camera();

	void addFocused(GameObject* _obj);
	void removeFocused(GameObject* _obj);
	void Update();

private:
	std::vector<GameObject*> focusedObjects;
	float minWidth, maxWidth;
	FloatRect borders;
	float ratio;
};
