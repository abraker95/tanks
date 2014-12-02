#include "Components.h"
#include "systems/UI_System.h"

UISystem::UISystem() {}
UISystem::~UISystem() {}


void UISystem::update(Environment* env)
{
	UserInterface* ui = env->get<UserInterface>();
	MouseControls* mouse = env->get<MouseControls>();

	for(unsigned i = 0; i<env->maxEntities(); i++)
	{
		if(env->hasComponents<UserInterface, MouseControls>(i))
		{
			// save the drag state if the button is pressed since it is going to be reset. 
			// This is needed for the logic handeling the case where the cursor goes fast
			// enough to escape the object's bounds
			bool drag = ui[i].enable.test(UserInterface::DRAG);
			
			ui[i].state.reset();
			if(ui[i].cursorOnThis)
			{
				if(ui[i].enable.test(UserInterface::HIGHLIGHT)) ui[i].state.set(UserInterface::HIGHLIGHT);
				if(mouse[i].pressState.test(MouseControls::LEFT))
				{
					if(ui[i].enable.test(UserInterface::PRESS)) ui[i].state.set(UserInterface::PRESS);
					if(ui[i].enable.test(UserInterface::DRAG))  ui[i].state.set(UserInterface::DRAG);
				    if(mouse[i].clickState.test(MouseControls::LEFT)) ui[i].action();
				}				 
			}

			if(mouse[i].pressState.test(MouseControls::LEFT) && drag)
				ui[i].state.set(UserInterface::DRAG);
		}
	}
}