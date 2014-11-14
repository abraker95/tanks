#include <cmath>
#include "Object.h"

Object::Object(const char* _filename, int _numFrames): Sprite()
{
 	numFrames = _numFrames;
	//solid = _hasPhysics;
 	currFrame = 0;
	destroyed = false;

	texture = new Texture[numFrames];

	// load the frames
	ostringstream convert;
	for(int frame = 0; frame<numFrames; frame++)
	{
		convert<<frame;
		texture[frame].loadFromFile(string(_filename)+"_"+convert.str()+".png");
     		PRINT_DEBUG(cout<<string(_filename)+"_"+convert.str()+".png"<<endl, HI_DEBUG);
	}

	const Vector2u size = texture[0].getSize();
	boundingCircleRadius = (float)MIN(size.x, size.y)/2.f;

	// set the origin to the center of the sprite by default
	setOrigin((float)size.x/2.f, (float)size.y/2.f);

	/// \TODO: put this in an update function
	setTexture(texture[currFrame]); 
}

Object::~Object()
{
    if(texture) delete[] texture;
}

void Object::destroy()
{
	destroyed = true;
}

bool Object::isDestroy() const
{
	return destroyed;
}

float Object::getBoundingCircleRadius() const
{
	return boundingCircleRadius;
}

Vector2f Object::getSize() const
{
	Vector2u size = texture[0].getSize();
	return Vector2f((float)size.x, (float)size.y);
}
