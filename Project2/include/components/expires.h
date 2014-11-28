#pragma once
#include "core/environment.h"

struct Expires : public Component<Expires>
{
	Expires();
	Expires(float _time);
	Expires(bool& _cond);
	Expires(float _time, bool& _cond);

	float time;
	bool condition;
};
