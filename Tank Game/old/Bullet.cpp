#include "Bullet.h"

Bullet::Bullet(Vector2f _pos, float _rot, GameObject* _parent)
	: GameObject(false, "Bullet", 1)
{
	setPosition(_pos);
	setRotation(_rot);

	velocity = 600.f;
	lifetime = 3.f; // 3 seconds lifetime
	parent = _parent;
}


Bullet::~Bullet()
{
 
}

void Bullet::Update(float _elapsedTime)
{
	elapsedTime = _elapsedTime;

	const float nextPosX = getPosition().x + cosf(DEG2RAD(getRotation())) * velocity * _elapsedTime,
			  	nextPosY = getPosition().y + sinf(DEG2RAD(getRotation())) * velocity * _elapsedTime;
	setPosition(nextPosX, nextPosY);

	// check bullet lifetime
	lifetime -= _elapsedTime;
	if(isDead())
		destroy();

	// check bullet collision with other objects
	/// \TODO: Event.create(REQ_VAR, Object, Environment);
	// Event.SelectEvent(REQ_VAR); // sets the pointer to the Object request from environment function
	const std::vector<GameObject*>& objects = Environment::getSingleton()->getObjects();
	for(size_t i=0;i<objects.size();i++)
	{
		// "living" object and colliding ?
		if(objects[i] != parent && objects[i]->hasHealth() && isCollidingWith(objects[i]))
		{
			objects[i]->addHealth(-20);
			cout<<"*hits*"<<endl;
			destroy();
			if(!objects[i]->hasHealth())
			{
				cout<<"BOOM!!"<<endl;
				objects[i]->destroy();
			}
			break;
		}
	}
}

bool Bullet::isDead() const
{
	return (lifetime <= 0.f);
}

void Bullet::UpdateUserInput() { /* no user input */ }
