#pragma once
#include <functional>
#include "core/environment.h"

class UserInterface: public Component<UserInterface>
{
	public:
		UserInterface();
		~UserInterface();

		//std::function<void*()> action;
};
