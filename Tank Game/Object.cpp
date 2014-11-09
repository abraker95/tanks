#include "Object.h"

Object::Object(const char* _filename, int _numframes): Sprite()
{
 	numFrames = _numframes;
 	currFrame = 0;

	texture = new Texture[numFrames];

	ostringstream convert;
	for(int frame = 0; frame<numFrames; frame++)
	{
		convert<<frame;
		texture[frame].loadFromFile(string(_filename)+"_"+convert.str()+".png");
     		PRINT_DEBUG(cout<<string(_filename)+"_"+convert.str()+".png"<<endl, HI_DEBUG);
	}
}

Object::~Object()
{
    if(texture) delete[] texture;
}

void Object::Render(RenderWindow* _window)
{
	if(++currFrame>=numFrames) currFrame = 0;
    PRINT_DEBUG(cout<<"currFrame: "<<currFrame<<endl, HI_DEBUG);

     if(texture)
        this->setTexture(texture[currFrame]);
	_window->draw(*this);
}
