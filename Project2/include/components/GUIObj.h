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
			SCROLLBAR,
			PANE,
			TEXTFIELD,
			VOID
		};

		GUIObj(Type _type, std::function<void*()> _action);

		Type type;
		std::function<void*()> action;
};

