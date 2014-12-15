#pragma once
#include "core/environment.h"

struct Expires : public Component<Expires>
{
	Expires(float _time);

	float time;
};
