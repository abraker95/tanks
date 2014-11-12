#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"

/** \NOTE: Bullet Game Object
	Spawn Case: 
		Tank: User presses space.
	Destroy Case:
		3 seconds lifetime
		\TODO: collision with a solid object
*/
class Bullet : public GameObject
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
