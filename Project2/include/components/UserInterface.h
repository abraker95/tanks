#pragma once
#include <functional>
#include "core/environment.h"

class UserInterface: public Component<UserInterface>
{
	#define UIstates 7

	public:
		UserInterface();
		UserInterface(std::bitset<UIstates> _enable, short _subMenu, std::function<void*()> _action = []()->void* { return nullptr; });
	//	UserInterface(std::function<void*()> _action, Type _type);
		~UserInterface();

		enum
		{
			HIGHLIGHT = 0,	// Highlights GUI area
			CLICK,			// Like press, only for 1 frame
			PRESS,			// True only when user presses on the GUI entity
			DRAG,			// 'nuff said
			TOGGLE,			// Toggle bit
			FOCUS,			// True if clicked and clicked nowhere else
			KEY				// Recieves key Input
			// Don't forget to change the number of UIstates
		};

		std::function<void*()> action;
		std::bitset<UIstates> enable, state; // enable: what states are allowed, state: current state
		bool cursorOnThis, show;
		short subMenu;
};