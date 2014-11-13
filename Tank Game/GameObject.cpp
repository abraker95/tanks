#include "GameObject.h"


GameObject::GameObject(bool _solid, const char* _filename, int _numFrames): Object(_filename, _numFrames)
{
	solid = _solid;
	setSpeed(0.f);
	setSpeed(0.f);
}


GameObject::~GameObject()
{}

float GameObject::getDist2(GameObject* _obj) const
{
	Vector2f delta = _obj->getPosition()-getPosition();
	return delta.x * delta.x+delta.y * delta.y;  // not needed to square root; keeping it in squared form
}

bool GameObject::isInRadius(GameObject* _obj, float _radius2) const
{
	float minDist = _obj->getBoundingCircleRadius()+_radius2;
	return (getDist2(_obj) < minDist * minDist);  // squared form
}

bool GameObject::isSolid() const
{
	return solid;
}

bool GameObject::isCollidingWith(GameObject* _obj) const
{
	if(solid && _obj->isSolid())
		return isInRadius(_obj, boundingCircleRadius);
	return false;
}

void GameObject::collisionFeedback(GameObject* _obj)
{
	if(solid && _obj->isSolid())
	{
		Vector2f delta = getPosition()-_obj->getPosition();
		float delta_length = sqrtf(delta.x * delta.x+delta.y * delta.y);

		// normalize delta
		delta.x /= delta_length;
		delta.y /= delta_length;

		// scale it to the minimum allowed distance
		delta.x *= (boundingCircleRadius+_obj->boundingCircleRadius);
		delta.y *= (boundingCircleRadius+_obj->boundingCircleRadius);

		Vector2f nextPos = _obj->getPosition()+delta;
		setPosition(nextPos);
	}
}
/*
Vector2f GameObject::getVelocityVector()
{

}*/
