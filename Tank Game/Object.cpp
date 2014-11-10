#include "Object.h"

Object::Object(const char* _filename, int _numFrames): Sprite()
{
 	numFrames = _numFrames;
 	currFrame = 0;

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

float Object::getDist2(Object* _obj) const
{
	Vector2f delta = _obj->getPosition() - getPosition();
	return delta.x * delta.x + delta.y * delta.y;  // not needed to square root; keeping it in squared form
}

bool Object::isInRadius(Object* _obj, float _radius) const
{
	float minDist = _obj->boundingCircleRadius + _radius;
	return (getDist2(_obj) <= minDist * minDist);  // squared form
}

bool Object::isCollidingWith(Object* _obj) const
{
	return isInRadius(_obj, boundingCircleRadius);
}

Object* Object::spawnObject(Object* _obj)
{
	childObjects.push_back(_obj);
	return childObjects[childObjects.size()-1];
}

	
void Object::UpdateChildObjs(RenderWindow* _window, float _elapsedTime)
{
	for(int i = 0; i<childObjects.size(); i++)
		if(childObjects[i]!=nullptr)
			childObjects[i]->Update(_window, _elapsedTime);
}
