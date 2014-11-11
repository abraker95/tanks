#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "utils.h"
#include "DEBUG.h"

using namespace sf;
using namespace std;

class Object: public Sprite
{
 public:
	Object(const char* _filename, int _numFrames = 1);
	virtual ~Object();

	virtual void Update(RenderWindow* _window, float _elapsedTime) = 0;

	// physics
	// the 2 suffix means squared
	float getDist2(Object* _obj) const;
	bool isInRadius(Object* _obj, float _radius2) const;
	bool isCollidingWith(Object* _obj) const;

 private:
 	int numFrames, currFrame;
	float boundingCircleRadius;

 protected:
    Texture* texture;

	virtual void UpdateUserInput() = 0;
	virtual void Render(RenderWindow* _window);
};
