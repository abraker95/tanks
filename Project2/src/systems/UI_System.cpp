#include "Components.h"
#include "systems/UI_System.h"

UISystem::UISystem() {}
UISystem::~UISystem() {}


void UISystem::update(Environment* env)
{
	UserInterface* ui = env->get<UserInterface>();

	for(unsigned i = 0; i<env->maxEntities(); i++)
	{
		if(env->hasComponents<Transform, UserInterface, MouseControls, RenderProperties>(i))
		{
			MouseControls* mouse = env->get<MouseControls>();

			ui[i].state.reset();
			if(ui[i].cursorOnThis)
			{
				if(ui[i].enable.test(UserInterface::HIGHLIGHT)) ui[i].state.set(UserInterface::HIGHLIGHT);
				if(mouse[i].pressState.test(MouseControls::LEFT))
				{
					if(ui[i].enable.test(UserInterface::PRESS)) ui[i].state.set(UserInterface::PRESS);
					if(mouse[i].clickState.test(MouseControls::LEFT)) ui[i].action();
				}				 
			}
		}
	}
}