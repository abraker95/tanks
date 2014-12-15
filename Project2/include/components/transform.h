#pragma once

#include "core/environment.h"
#include "math/vector.h"

struct Transform : public Component<Transform>
{
	Transform(Vec2f pos, float rot);

	Vec2f pos;
	float rot;

	// only geometric transformations here - Sherushe
	/*
	/// \TODO: Implement a type to keep track of prev vals. This way the renderer will check against 
	///        prev states to figure out what needs to be rerendered.  DVar = Delta Variable
	///        ex: DVar<int>  state
	///            members: T curVal, preVal;
	///                     bool Changed();
	bool update; // does the entity need redrawing?
	*/
};
