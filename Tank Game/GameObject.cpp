#include "GameObject.h"


GameObject::GameObject(bool _solid, const char* _filename, int _numFrames): Object(_filename, _numFrames)
{
	solid = _solid;
	health = 0;
	maxHealth = 0;
	speed = 0.f;
}


GameObject::~GameObject()
{}

float GameObject::getDist2(GameObject* _obj) const
{
	Vector2f delta = _obj->getPosition()-getPosition();
	return delta.x * delta.x +delta.y * delta.y;  // not needed to square root; keeping it in squared form
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
		return isInRadius(_obj, boundingCircleRadius);
}

// This funtion just return the position next to the to be collided oject
void GameObject::collisionFeedback(GameObject* _obj)
{
		Vector2f delta = getPosition()-_obj->getPosition();
		float delta_length = sqrtf(delta.x*delta.x + delta.y*delta.y);

		// normalize delta
		delta.x /= delta_length;
		delta.y /= delta_length;

		// scale it to the minimum allowed distance
		delta.x *= (boundingCircleRadius+_obj->boundingCircleRadius);
		delta.y *= (boundingCircleRadius+_obj->boundingCircleRadius);

		Vector2f nextPos = _obj->getPosition()+delta;
		setPosition(nextPos);
}

Vector2f GameObject::getVelocityVector(float _elapsedTime)
{
	float nextAngle = getRotation() + _elapsedTime * angleSpeed;

	Vector2f nextPos(cosf(DEG2RAD(nextAngle)+PI/2) * speed * _elapsedTime,
					 sinf(DEG2RAD(nextAngle)+PI/2) * speed * _elapsedTime);

	return nextPos;
}

Vector2f GameObject::getSurfaceTangentVector(float _elapsedTime)
{
	float nextAngle = getRotation()+_elapsedTime * angleSpeed,
			radians = DEG2RAD(nextAngle)+PI/2;

	Vector2f surfaceNormal = Vector2f(boundingCircleRadius*cosf(radians),
									  boundingCircleRadius*sinf(radians)),
		// take the perpendicular result
			 surfaceTangent = Vector2f(surfaceNormal.x*cos(radians)-surfaceNormal.y*sin(radians),
									   surfaceNormal.x*sin(radians)+surfaceNormal.y*cos(radians));

	return surfaceTangent;
}

bool GameObject::isCollision(GameObject* _obj, float _elapsedTime)
{
	Vector2f obj1SurfaceTangent, obj2SurfaceTangent,
			 obj1VelocityVector, obj2VelocityVector;

	obj1VelocityVector = getVelocityVector(_elapsedTime);
	obj1SurfaceTangent = getSurfaceTangentVector(_elapsedTime);
	obj2SurfaceTangent = _obj->getSurfaceTangentVector(_elapsedTime);

	// intersection between the Volocity vectors
	Vector2f lnSegobj1VelVec[2] = {getPosition(), getPosition()+getVelocityVector(_elapsedTime)},
		     lnSegobj2VelVec[2] = {_obj->getPosition(), _obj->getPosition()+_obj->getVelocityVector(_elapsedTime)};

	float slopeobj1VelVec = (lnSegobj1VelVec[0].y-lnSegobj1VelVec[1].y) / (lnSegobj1VelVec[0].x-lnSegobj1VelVec[1].x),
	      slopeobj2VelVec = (lnSegobj2VelVec[0].y-lnSegobj2VelVec[1].y) / (lnSegobj2VelVec[0].x-lnSegobj2VelVec[1].x),
		  Bobj1VelVec = slopeobj1VelVec*lnSegobj1VelVec[0].x+lnSegobj1VelVec[0].y,
		  Bobj2VelVec = slopeobj2VelVec*lnSegobj2VelVec[0].x+lnSegobj2VelVec[0].y;

	if(slopeobj1VelVec == slopeobj2VelVec) // they are parallel
	{
		// are they colinear?
	}
	else
	{
		float xIntersection = (Bobj2VelVec-Bobj1VelVec)/(slopeobj1VelVec-slopeobj2VelVec),
			  yIntersection = slopeobj1VelVec*xIntersection+Bobj1VelVec;
	}
	return false;  // \TODO: WIP
}
