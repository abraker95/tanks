#pragma once
#include <SFML/Graphics.hpp>
#include "Object.h"

/** \NOTE: Bullet Game Object
	Spawn Case: 
		Tank: User presses space.
	Destroy Case:
		3 seconds lifetime
		\TODO: collision with a solid object
*/
class Bullet : public Object
{
public:
	Bullet(std::vector<Object*>* _objs, Vector2f _pos, float _rot);
	virtual ~Bullet();

	void Update(float _elapsedTime);
	void Render(RenderWindow* _window);
	bool isDead() const;

private:
	float velocity;
	float lifetime;
	
	void UpdateUserInput();
};
