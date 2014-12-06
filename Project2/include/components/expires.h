#pragma once
#include "core/environment.h"

struct Expires : public Component<Expires>
{
	Expires(float _time);
	Expires(bool& _condition);
	Expires(float _time, bool& _condition);

	float time;
	bool condition;
};
