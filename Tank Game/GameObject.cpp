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

bool GameObject::intersectRectRect(GameObject* _obj1, GameObject* _obj2)
{
	const Vector2f& pos1 = _obj1->getPosition();
	const Vector2f& pos2 = _obj2->getPosition();
	const Vector2f  size1 = _obj1->getSize();
	const Vector2f  size2 = _obj2->getSize();

	// x axis overlap ?
	if(pos1.x + size1.x/2.f < pos2.x - size2.x/2.f || pos2.x + size2.x/2.f < pos1.x - size1.x/2.f)
		return false;
	// x axis overlap ?
	if(pos1.y + size1.y/2.f < pos2.y - size2.y/2.f || pos2.y + size2.y/2.f < pos1.y - size1.y/2.f)
		return false;
	
	return true;
}

bool GameObject::intersectCircleCircle(GameObject* _obj1, GameObject* _obj2)
{
	return _obj1->isInRadius(_obj2, _obj2->boundingCircleRadius);
}

bool GameObject::intersectCircleRect(GameObject* _obj1, GameObject* _obj2)
{
	if(intersectRectPoint(_obj2, _obj1->getPosition()))
		return true;

	// _obj2 is the rect
	const Vector2f& pos2 = _obj1->getPosition();
	const Vector2f  size2 = _obj2->getSize();

	// the four corners
	Vector2f c1(pos2.x - size2.x/2, pos2.y - size2.y/2);
	Vector2f c2(pos2.x + size2.x/2, pos2.y - size2.y/2);
	Vector2f c3(pos2.x + size2.x/2, pos2.y + size2.y/2);
	Vector2f c4(pos2.x - size2.x/2, pos2.y + size2.y/2);

	return (intersectCircleLine(_obj1, c1, c2)
		|| intersectCircleLine(_obj1, c2, c3)
		|| intersectCircleLine(_obj1, c3, c4)
		|| intersectCircleLine(_obj1, c4, c1));
}

bool GameObject::intersectCirclePoint(GameObject* _obj1, const Vector2f& _pt)
{
	Vector2f delta = _pt - _obj1->getPosition();
	return delta.x * delta.x + delta.y * delta.y < _obj1->boundingCircleRadius * _obj1->boundingCircleRadius;
}

bool GameObject::intersectCircleLine(GameObject* _obj1, const Vector2f& _l1, const Vector2f& _l2)
{
	// project the center of the circle onto the line
	Vector2f lineDir = _l2 - _l1, unit, cl = _obj1->getPosition() - _l1;
	float length = sqrtf(lineDir.x * lineDir.x + lineDir.y * lineDir.y);

	unit.x = lineDir.x / length;
	unit.y = lineDir.y / length;

	float projDist = cl.x * unit.x + cl.y * cl.y;
	Vector2f proj = projDist * unit + _l1;

	return (projDist >= 0.f && projDist <= length && intersectCirclePoint(_obj1, proj));
}

bool GameObject::intersectRectPoint(GameObject* _obj1, const Vector2f& _pt)
{
	const Vector2f& pos1 = _obj1->getPosition();
	const Vector2f size1 = _obj1->getSize();

	return ((_pt.x >= pos1.x - size1.x/2 &&  _pt.x <= pos1.x + size1.x/2) ||
		(_pt.y >= pos1.y - size1.y/2 &&  _pt.y <= pos1.y + size1.y/2));
}

Vector2f GameObject::responseCircleRect(GameObject* _obj1, GameObject* _obj2)
{
	const Vector2f& c1 = _obj1->getPosition();
	float r = _obj1->boundingCircleRadius;
	const Vector2f& c2 = _obj2->getPosition();
	const Vector2f s = _obj2->getSize();

	Vector2f delta;

	float dx1 = (c1.x + r) - (c2.x - s.x/2.f);
	float dx2 = (c2.x + s.x/2.f) - (c1.x - r);

	delta.x = MIN(dx1, dx2);	

	float dy1 = (c1.y + r) - (c2.y - s.y/2.f);
	float dy2 = (c2.y + s.y/2.f) - (c1.y - r);

	delta.y = MIN(dy1, dy2);	
	
	if(delta.x < delta.y) delta.y = 0.f;
	else delta.x = 0.f;
	
	return delta;
}

Vector2f GameObject::responseCircleCircle(GameObject* _obj1, GameObject* _obj2)
{
	Vector2f delta = _obj1->getPosition() - _obj2->getPosition();
	float delta_length = sqrtf(delta.x*delta.x + delta.y*delta.y);

	// normalize delta
	delta.x /= delta_length;
	delta.y /= delta_length;

	// scale it to the minimum allowed distance
	delta.x *= (_obj1->boundingCircleRadius + _obj2->boundingCircleRadius);
	delta.y *= (_obj2->boundingCircleRadius + _obj2->boundingCircleRadius);

	return delta;
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
