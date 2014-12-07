#pragma once
#include <functional>
#include "core/environment.h"

class UserInterface: public Component<UserInterface>
{
	#define UIstates 5

	public:
		UserInterface();
		UserInterface(std::bitset<UIstates> _enable, std::function<void*()> *_action = nullptr);
	//	UserInterface(std::function<void*()> _action, Type _type);
		~UserInterface();

		enum
		{
			HIGHLIGHT = 0,
			CLICK,
			PRESS,
			DRAG,
			TOGGLE
		};

		std::function<void*()> *action;
		std::bitset<UIstates> enable,state;
		bool cursorOnThis;
};