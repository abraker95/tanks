#include "Object.h"

Object::Object(const char* _filename, int _numframes)
{
 	numFrames = _numframes;
 	currFrame = 0;

	texture = new Texture[numFrames];
	sprite = new Sprite[numFrames];

	ostringstream convert;
	for(int frame = 0; frame<numFrames; frame++)
	{
		convert<<frame;
		texture[frame].loadFromFile(string(_filename)+"_"+convert.str()+".png");
     		PRINT_DEBUG(cout<<string(_filename)+"_"+convert.str()+".png"<<endl, HI_DEBUG);

		// only copys the reference, doesn't copy the texture
		sprite[frame] = Sprite(texture[frame]);
	}
}

Object::~Object()
{
	if(texture) delete[] texture;
	if(sprite) 	delete[] sprite;
}

void Object::setPos(int _xpos, int _ypos)
{
	for(int frame = 0; frame<numFrames; frame++)
		sprite[frame].setPosition(_xpos, _ypos);
}

void Object::Render(RenderWindow* window)
{
	if(++currFrame>=numFrames) currFrame = 0;
		window->draw(sprite[currFrame]);
}
