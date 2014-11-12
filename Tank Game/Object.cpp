#include <cmath>
#include "Object.h"

Object::Object(bool _hasPhysics, const char* _filename, int _numFrames): Sprite()
{
 	numFrames = _numFrames;
 	currFrame = 0;
	destroy = false;

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

	hasPhysics = _hasPhysics;
}

Object::~Object()
{
    if(texture) delete[] texture;
}

bool Object::isDestroy() const
{
	return destroy;
}

void Object::Render(RenderWindow* _window)
{
	/*
	if(++currFrame>=numFrames) currFrame = 0;
    PRINT_DEBUG(cout<<"currFrame: "<<currFrame<<endl, HI_DEBUG);

     if(texture)
        setTexture(texture[currFrame]);
	
		_window->draw(*this); <-- doesn't work with multiple inheritance
	*/
}

float Object::getDist2(Object* _obj) const
{
	Vector2f delta = _obj->getPosition() - getPosition();
	return delta.x * delta.x + delta.y * delta.y;  // not needed to square root; keeping it in squared form
}

bool Object::isInRadius(Object* _obj, float _radius) const
{
	float minDist = _obj->boundingCircleRadius + _radius;
	return (getDist2(_obj) < minDist * minDist);  // squared form
}

bool Object::isCollidingWith(Object* _obj) const
{
	if(hasPhysics && _obj->hasPhysics)
		return isInRadius(_obj, boundingCircleRadius);
	return false;
}

void Object::collisionFeedback(Object* _obj)
{
	if(hasPhysics && _obj->hasPhysics)
	{
		Vector2f delta = getPosition() - _obj->getPosition();
		float delta_length = sqrtf(delta.x * delta.x + delta.y * delta.y);

		// normalize delta
		delta.x /= delta_length;
		delta.y /= delta_length;

		// scale it to the minimum allowed distance
		delta.x *= (boundingCircleRadius + _obj->boundingCircleRadius);
		delta.y *= (boundingCircleRadius + _obj->boundingCircleRadius);

		Vector2f nextPos = _obj->getPosition() + delta;
		setPosition(nextPos);
	}
}
