#pragma once
#include <sfml/System/Vector2.hpp>
#include "utils.h"
using namespace sf;

/// \TODO: make this a template class
class VectorLN
{
 public:
	VectorLN();
	VectorLN(Vector2f _vec);
	VectorLN(float _xMag, float _yMag);
	VectorLN(float _mag);

	virtual ~VectorLN();

	VectorLN operator+(VectorLN _vec);
	VectorLN operator-(VectorLN _vec);
	VectorLN operator*(VectorLN _vec);
	VectorLN operator/(VectorLN _vec);

	VectorLN operator+=(VectorLN _vec);
	VectorLN operator-=(VectorLN _vec);
	VectorLN operator*=(VectorLN _vec);
	VectorLN operator/=(VectorLN _vec);

	VectorLN operator=(VectorLN _vec);
	VectorLN operator==(VectorLN _vec);
	VectorLN operator()();

	VectorLN Normalize();
	VectorLN getPerpendicular();
	float getSlope();

	bool isIntersection(VectorLN _vec);
	bool isParallel(VectorLN _vec);
	VectorLN getIntersection(VectorLN _vec);

	// Why doesn't it accept Vector2f?!?
	operator Vector2<float>();

	// I'm getting errors here no matter what I do
//	operator Vector2<T> const;// { return VectorLN(x, y); }

	float x, y;
	//VectorLN Rotate(T _deg);
};

