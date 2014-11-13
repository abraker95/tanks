#include "Bullet.h"

Bullet::Bullet(Vector2f _pos, float _rot)
	: GameObject(false, "Bullet", 1)
{
	setPosition(_pos);
	setRotation(_rot);

	velocity = 800.f;
	lifetime = 3.f; // 3 seconds lifetime
}


Bullet::~Bullet()
{
 
}

void Bullet::Update(float _elapsedTime)
{
	const float nextPosX = getPosition().x + cosf(DEG2RAD(getRotation())) * velocity * _elapsedTime,
			  	nextPosY = getPosition().y + sinf(DEG2RAD(getRotation())) * velocity * _elapsedTime;
	setPosition(nextPosX, nextPosY);

	lifetime -= _elapsedTime;
	if(isDead())
		destroy = true;
}

bool Bullet::isDead() const
{
	return (lifetime <= 0.f);
}

void Bullet::UpdateUserInput() { /* no user input */ }
