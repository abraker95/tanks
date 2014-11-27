#pragma once
#include "core/environment.h"

struct Expires : public Component<Expires>
{
	Expires();
	Expires(float time);

	float time;
};
