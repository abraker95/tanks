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

    void Render(RenderWindow* _window);

 private:
 	int numFrames, currFrame;
    Texture* texture;
};
