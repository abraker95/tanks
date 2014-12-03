#pragma once
#include <functional>
#include "core/environment.h"

class UISystem
{
public:
	UISystem();
	UISystem(Environment* env);
	~UISystem();

	void update(Environment* env);
};