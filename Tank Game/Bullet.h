#pragma once
#include <SFML/Graphics.hpp>
#include "Object.h"

class Bullet : public Object
{
public:
	Bullet(Vector2f _pos, float _rot);
	virtual ~Bullet();

	void Update(float _elapsedTime);
	void Render(RenderWindow* _window);
	bool isDead() const;

private:
	float velocity;
	float lifetime;
	
	void UpdateUserInput();
};
