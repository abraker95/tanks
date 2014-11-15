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

float GameObject::getNextAngle()
{
	return getRotation() + elapsedTime*angleSpeed;
}

VectorLN GameObject::getVelocityVector()
{
	return VectorLN(cosf(DEG2RAD(getNextAngle())+PI/2) * speed * elapsedTime,
			        sinf(DEG2RAD(getNextAngle())+PI/2) * speed * elapsedTime);
}

VectorLN GameObject::getSurfaceNormalVector()
{
	return VectorLN(boundingCircleRadius * getVelocityVector().Normalize().x,
					boundingCircleRadius * getVelocityVector().Normalize().y);

}

VectorLN GameObject::getSurfaceTangentVector()
{
	return VectorLN(2*boundingCircleRadius*cosf(DEG2RAD(getNextAngle())+PI/2), 
		            2*boundingCircleRadius*sinf(DEG2RAD(getNextAngle())+PI/2)).getPerpendicular();
}

bool GameObject::isCollision(GameObject* _obj)
{
	VectorLN obj1SurfaceTangent, obj2SurfaceTangent,
			 obj1VelocityVector, obj2VelocityVector;

	VectorLN surfaceNormal = VectorLN(boundingCircleRadius * getVelocityVector().Normalize().x,
									  boundingCircleRadius * getVelocityVector().Normalize().y);

	obj1VelocityVector = getVelocityVector();
	obj1SurfaceTangent = getSurfaceTangentVector();
	obj2SurfaceTangent = _obj->getSurfaceTangentVector();

	// intersection between the Volocity vectors
	VectorLN lnSegobj1VelVec[2] = {VectorLN(getPosition().x, getPosition().y), 
			   				       VectorLN(getPosition().x, getPosition().y)+getVelocityVector()},
			 lnSegobj2VelVec[2] = {VectorLN(_obj->getPosition().x, _obj->getPosition().y),
								   VectorLN(_obj->getPosition().x, _obj->getPosition().y)+_obj->getVelocityVector()};

// debug -------------------------------------------------------------------------------------------
	sf::Vertex line[] =
	{
		sf::Vertex(VectorLN(getPosition())+surfaceNormal),
		sf::Vertex(VectorLN(getPosition())+surfaceNormal+getVelocityVector())
	};

	sf::Vertex line1[] =
	{
		sf::Vertex(VectorLN(getPosition())),
		sf::Vertex(VectorLN(getPosition())+getSurfaceTangentVector())
		//sf::Vertex(VectorLN(getPosition())+VectorLN(boundingCircleRadius*cosf(DEG2RAD(getNextAngle())+PI/2), boundingCircleRadius*sinf(DEG2RAD(getNextAngle())+PI/2)).getPerpendicular()+surfaceNormal),
		//sf::Vertex(VectorLN(getPosition())-VectorLN(boundingCircleRadius*cosf(DEG2RAD(getNextAngle())+PI/2), boundingCircleRadius*sinf(DEG2RAD(getNextAngle())+PI/2)).getPerpendicular()+surfaceNormal)
	};
	
	extern RenderWindow* window;
	window->draw(line, 2, sf::Lines);
	window->draw(line1, 2, sf::Lines);

	CircleShape shape(boundingCircleRadius);
	shape.setOutlineThickness(2);
	shape.setFillColor(Color(0, 0, 0,0));
	shape.setOutlineColor(Color(150, 50, 250));
	shape.setPosition(Vector2f(getPosition().x-boundingCircleRadius, getPosition().y-boundingCircleRadius));
	window->draw(shape);

// debug end-----------------------------------------------------------------------------------------------------------------------

	float slopeobj1VelVec = lnSegobj1VelVec[1].getSlope(),
		  slopeobj2VelVec = lnSegobj2VelVec[1].getSlope();

	if(lnSegobj1VelVec[1].isParallel(lnSegobj2VelVec[1])) // are they are parallel?
	{
		// are the objects going to overlap?
		if(BTWN(-getBoundingCircleRadius()+lnSegobj1VelVec[1].x, -getBoundingCircleRadius()+lnSegobj2VelVec[1].x, getBoundingCircleRadius()+lnSegobj1VelVec[1].x)
		&& BTWN(-getBoundingCircleRadius()+lnSegobj1VelVec[1].y, -getBoundingCircleRadius()+lnSegobj2VelVec[1].y, getBoundingCircleRadius()+lnSegobj1VelVec[1].y))
			return true;
		else
			return false;
	}
	/**else 
	{
		/// \TODO: To be implement in the vector class

		 // calculates the point of collision
		/// \TODO: Take care of collisions that are far enough for data type overflow
		float xIntersection = (lnSegobj1VelVec[0].y - lnSegobj2VelVec[0].y + slopeobj1VelVec*lnSegobj1VelVec[0].x - slopeobj2VelVec*lnSegobj2VelVec[0].x) / (slopeobj1VelVec-slopeobj2VelVec),
			  yIntersection = slopeobj1VelVec*(xIntersection-lnSegobj1VelVec[0].x)+lnSegobj1VelVec[0].y;

		 // checks to make sure the collision is within scope
		if(BTWN(MIN(lnSegobj1VelVec[0].x, lnSegobj2VelVec[0].x), xIntersection, MAX(lnSegobj1VelVec[1].x, lnSegobj2VelVec[1].x))
		&& BTWN(MIN(lnSegobj1VelVec[0].y, lnSegobj2VelVec[0].y), yIntersection, MAX(lnSegobj1VelVec[1].y, lnSegobj2VelVec[1].y)))
		{
		 // \TODO: Retrurn point of collison
		}
	}*/
	return true; // if not parallel, then according to 2D geometry, they will intersect at some point
}
