#pragma once
#include <functional>
#include "core/environment.h"

class GUIObj: public Component<GUIObj>
{
	public:
		enum Type
		{
			BUTTON,
			SLIDER,
			RADIO,
			SCROLLBAR
		};

		GUIObj();
		GUIObj(Type _type, std::function<void*()> _action);
		~GUIObj();

		Type type;
		std::function<void*()> action;
};

