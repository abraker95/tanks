#include <cmath>
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

	const Vector2u size = texture[0].getSize();
	boundingSphereRadius = (float)MIN(size.x, size.y);
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
        setTexture(texture[currFrame]);
	_window->draw(*this);
}

float Object::getDist(Object* _obj) const
{
	Vector2f delta = _obj->getPosition() - getPosition();
	return sqrtf(delta.x * delta.x + delta.y * delta.y);
}

bool Object::isCollidingWith(Object* _obj) const
{
	return (getDist(_obj) <= _obj->boundingSphereRadius + boundingSphereRadius);
}

bool Object::isInRadius(Object* _obj, float _radius) const
{
	return (getDist(_obj) <= _obj->boundingSphereRadius + _radius);
}
