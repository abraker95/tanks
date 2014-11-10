#pragma once
#include <SFML/Graphics.hpp>
#include "utils.h"
#include "DEBUG.h"
using namespace sf;

class Object: public Sprite
{
 public:
	Object(const char* _filename, int _numframes = 1);
	virtual ~Object();

    virtual void Render(RenderWindow* _window);

	// physics
	float getDist(Object* _obj) const;
	bool isCollidingWith(Object* _obj) const;
	bool isInRadius(Object* _obj, float _radius) const;

 private:
 	int numFrames, currFrame;
	float boundingSphereRadius;

 protected:
    Texture* texture;
};
