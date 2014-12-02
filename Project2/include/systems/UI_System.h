#pragma once
#include <functional>
#include "core/environment.h"

class UISystem
{
public:
	UISystem();
	~UISystem();

	void update(Environment* env);
};