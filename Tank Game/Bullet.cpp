#include "Bullet.h"


Bullet::Bullet(Vector2f _pos, float _rot)
	: Object("Bullet", 1)
{
	setPosition(_pos);
	setRotation(_rot);
}


Bullet::~Bullet()
{
 
}

void Bullet::Update(RenderWindow* _window, float _elapsedTime)
{
	const float nextPosX = getPosition().x + cosf(DEG2RAD(getRotation())) * velocity * _elapsedTime,
			  	nextPosY = getPosition().y + sinf(DEG2RAD(getRotation())) * velocity * _elapsedTime;
	setPosition(nextPosX, nextPosY);

	Render(_window);
}

void Bullet::UpdateUserInput() { /* no user input */ }
