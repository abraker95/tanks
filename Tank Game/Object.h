#pragma once
#include <SFML/Graphics.hpp>
#include "utils.h"
#include "DEBUG.h"

class Environment;

using namespace sf;
using namespace std;

class Object: public Sprite
{
 public:
	Object(const char* _filename, int _numFrames = 1);
	virtual ~Object();

	virtual void Update(float _elapsedTime) = 0;
	virtual void Render(RenderWindow* _window) = 0;

	bool isDestroy() const;
	float getBoundingCircleRadius() const;

	bool isSolid() const;
	void collisionFeedback(Object* _obj);
	Vector2f getVelocityVector();
 
 protected:
	bool destroy;
	Texture* texture;
	float boundingCircleRadius;


	virtual void UpdateUserInput() = 0;

 private:
	 /// \TODO: implemnt velocity vectors
 	int numFrames, currFrame;
};
