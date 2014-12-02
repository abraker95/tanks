#pragma once
#include <functional>
#include "core/environment.h"

class UserInterface: public Component<UserInterface>
{
	#define states 4

	public:
		UserInterface();
		UserInterface(std::function<void*()> _action, std::bitset<states> _enable);
		~UserInterface();

		enum
		{
			HIGHLIGHT = 0,
			CLICK,
			PRESS,
			DRAG
		};

		std::function<void*()> action;
		std::bitset<states> enable,state;
		bool cursorOnThis;
};