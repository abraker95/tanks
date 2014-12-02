#pragma once
#include <functional>
#include "core/environment.h"

class UserInterface: public Component<UserInterface>
{
	public:
		UserInterface();
		UserInterface(std::function<void*()> _action, std::bitset<3> _enable);
		~UserInterface();

		enum
		{
			HIGHLIGHT = 0,
			CLICK,
			PRESS
		};

		std::function<void*()> action;
		std::bitset<3> enable,state;
		bool call, cursorOnThis;

};