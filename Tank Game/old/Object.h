#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <sstream>
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

	int camNumFocus;

	void destroy();
	bool isDestroy() const;
	float getBoundingCircleRadius() const;
	const Vector2f getSize() const;
	void collisionFeedback(Object* _obj);

 protected:
	bool destroyed;
	Texture* texture;

	bool boundingCircle;
	float boundingCircleRadius,
		  elapsedTime;

	virtual void UpdateUserInput() = 0;

 private:
 	int numFrames, currFrame;
};
