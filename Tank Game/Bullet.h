#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "Environment.h"

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
	Bullet(Vector2f _pos, float _rot, GameObject* _parent);
	virtual ~Bullet();

	void Update(float _elapsedTime);
	bool isDead() const;

private:
	float velocity;
	float lifetime;
	
	void UpdateUserInput();
	GameObject* parent;
};
