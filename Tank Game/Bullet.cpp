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
	/// \TODO: Why aren't the bullets moving?
	const int nextPosX = getPosition().x + cosf(getRotation()) * velocity * _elapsedTime,
			  nextPosY = getPosition().y + sinf(getRotation()) * velocity * _elapsedTime;
	setPosition(nextPosX, nextPosY);

	Render(_window);
}

void Bullet::UpdateUserInput() { /* no user input */ }