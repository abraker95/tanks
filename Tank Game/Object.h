#pragma once
#include <SFML/Graphics.hpp>
#include "utils.h"
#include "DEBUG.h"
using namespace sf;

class Object
{
 public:
	Object(RenderWindow* _window, const char* _filename, int _numframes = 1);
	virtual ~Object();

	void Render();
	void setPos(int _xpos, int _ypos);
	int getX();
	int getY();

 private:
 	int numFrames, currFrame;
	Sprite* sprite;
	Texture* texture;
};
