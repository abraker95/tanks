#pragma once
#include <SFML/Graphics.hpp>
#include <functional> 
#include <vector>
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

	// physics
	// the 2 suffix means squared
	float getDist2(Object* _obj) const;
	bool isInRadius(Object* _obj, float _radius2) const;
	bool isCollidingWith(Object* _obj) const;
	bool isDestroy() const;

 private:
 	int numFrames, currFrame;
	float boundingCircleRadius;

 protected:
	bool destroy;
    Texture* texture;
	virtual void UpdateUserInput() = 0;
};