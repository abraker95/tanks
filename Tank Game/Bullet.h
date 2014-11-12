#pragma once
#include <SFML/Graphics.hpp>
#include "Object.h"

class Bullet: public Object
{
public:
	Bullet(Vector2f _pos, float _rot);
	virtual ~Bullet();

	void Update(RenderWindow* _window, float _elapsedTime);

private:
	const float velocity = 800.f;
	
	void UpdateUserInput();
};