#include "VectorLN.h"

VectorLN::VectorLN() { x = 0; y = 0; }

VectorLN::VectorLN(float _x, float _y) { x = _x; y = _y; }
VectorLN::VectorLN(float _mag) { x= _mag; y = _mag; }
VectorLN::VectorLN(Vector2f _vec) { x = _vec.x; y = _vec.y; };

VectorLN::~VectorLN() {}

VectorLN VectorLN::operator+(VectorLN _vec)  { return VectorLN(x+_vec.x, y+_vec.y); }
VectorLN VectorLN::operator-(VectorLN _vec)  { return VectorLN(x-_vec.x, y-_vec.y); }
VectorLN VectorLN::operator*(VectorLN _vec)  { return VectorLN(x*_vec.x, y*_vec.y); }
VectorLN VectorLN::operator/(VectorLN _vec)  { return VectorLN(x/_vec.x, y/_vec.y); }

VectorLN VectorLN::operator=(VectorLN _vec)  { x = _vec.x; y = _vec.y; return *this; }
VectorLN VectorLN::operator==(VectorLN _vec) { return (x==_vec.x  && y==_vec.y); }

VectorLN VectorLN::operator+=(VectorLN _vec) { *this = *this+_vec; return VectorLN(x+_vec.x, y+_vec.y); }
VectorLN VectorLN::operator-=(VectorLN _vec) { *this = *this-_vec; return VectorLN(x+_vec.x, y+_vec.y); }
VectorLN VectorLN::operator*=(VectorLN _vec) { *this = *this*_vec; return VectorLN(x+_vec.x, y+_vec.y); }
VectorLN VectorLN::operator/=(VectorLN _vec) { *this = *this/_vec; return VectorLN(x+_vec.x, y+_vec.y); }

VectorLN VectorLN::operator()() 
{
	return *this;
}

VectorLN VectorLN::Normalize() 
{
	if(x == 0)	  return VectorLN(x/ABS(y), y/ABS(y));
	else if(y==0) return VectorLN(x/ABS(x), y/ABS(x));
	else          return VectorLN(x/MAX(ABS(x), ABS(y)), y/MAX(ABS(x), ABS(y)));
}

VectorLN VectorLN::getPerpendicular() 
{
	return VectorLN(y, -x);
}

float VectorLN::getSlope()
{
	return y/x;
}

 VectorLN VectorLN::getIntersection(VectorLN _vec) 
{
	float xRet = (y - _vec.y + getSlope()*x - _vec.getSlope()*_vec.x)/(getSlope() - _vec.getSlope()),
	      yRet = getSlope()*(xRet - x) + y;
	return VectorLN(xRet, yRet);
}

bool VectorLN::isIntersection(VectorLN _vec)
{
	return getSlope()!=_vec.getSlope();
}

bool isParallel(VectorLN _vec);
 bool VectorLN::isParallel(VectorLN _vec)
{
	return getSlope()==_vec.getSlope();
}

 VectorLN::operator Vector2<float>()
{
	return Vector2<float>(x, y);
}

// VectorLN::VectorLN Rotate(T _deg) {}