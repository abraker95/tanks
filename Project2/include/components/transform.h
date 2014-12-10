#pragma once

#include "core/environment.h"

struct Transform : public Component<Transform>
{
	Transform(float x, float y, float rot);

	float x, y;
	float rot;

	/// \TODO: Implement a type to keep track of prev vals. This way the renderer will check against 
	///        prev states to figure out what needs to be rerendered.  DVar = Delta Variable
	///        ex: DVar<int>  state
	///            members: T curVal, preVal;
	///                     bool Changed();
	bool update; // does the entity need redrawing?
};
