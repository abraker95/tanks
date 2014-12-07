#include "Components.h"
#include "systems/UI_System.h"

UISystem::UISystem() {}

UISystem::UISystem(Environment* env)
{
	// Factory based procedure for constructing GUIObjects
	GUIObj* GUIobjs = env->get<GUIObj>();

	for(unsigned i = 0; i<env->maxEntities(); i++)
	{
		if(env->hasComponents<GUIObj>(i))
		{
			// Button
			// \NOTE: User Interfce action needs to be set externally
			if(GUIobjs[i].type==GUIObj::BUTTON)
			{
				/// \TODO: add logic for if it already has the component
				assert(!env->hasComponents<UserInterface>(i));
				env->addComponents<UserInterface>(i, UserInterface(
					std::bitset<UIstates>(1<<UserInterface::HIGHLIGHT|1<<UserInterface::CLICK|1<<UserInterface::PRESS), &GUIobjs[i].action));
			}
		}
	}
}

UISystem::~UISystem() {}


void UISystem::update(Environment* env)
{
	UserInterface* ui = env->get<UserInterface>();
	MouseControls* mouse = env->get<MouseControls>();

	for(unsigned i = 0; i<env->maxEntities(); i++)
	{
		// \TODO: Should the press flag be still on if the cursor moves from the object while user is still
		//        holding the button or not?
		if(env->hasComponents<UserInterface, MouseControls>(i))
		{
			// save the drag state if the button is pressed since it is going to be reset. 
			// This is needed for the logic handeling the case where the cursor goes fast
			// enough to escape the object's bounds
			bool drag = ui[i].enable.test(UserInterface::DRAG),
				 toggle = ui[i].enable.test(UserInterface::TOGGLE);

			ui[i].state.reset();
			if(ui[i].cursorOnThis)
			{
				if(ui[i].enable.test(UserInterface::HIGHLIGHT)) ui[i].state.set(UserInterface::HIGHLIGHT);
				if(mouse[i].pressState.test(MouseControls::LEFT))
				{
					if(ui[i].enable.test(UserInterface::TOGGLE) && !toggle) ui[i].state.set(UserInterface::TOGGLE);
					if(ui[i].enable.test(UserInterface::PRESS)) ui[i].state.set(UserInterface::PRESS);
					if(ui[i].enable.test(UserInterface::DRAG))  ui[i].state.set(UserInterface::DRAG);
				    if(mouse[i].clickState.test(MouseControls::LEFT)) (*ui[i].action)();
				}				 
			}

			if(mouse[i].pressState.test(MouseControls::LEFT) && drag)
				ui[i].state.set(UserInterface::DRAG);
		}
	}
}