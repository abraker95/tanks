#include "Components.h"
#include "systems/UI_System.h"

UISystem::UISystem() {}

UISystem::UISystem(Environment* env)
{
	// Factory based procedure for constructing GUIObjects
	auto GUIobjs = env->get<GUIObj>();
	auto sprite = env->get<Sprite>();
	auto trans = env->get<Transform>();
	auto labels = env->get<Label>();

	for(unsigned i = 0; i<env->maxEntities(); i++)
	{
		if(env->hasComponents<GUIObj>(i))
		{
			// Button
			// \NOTE: User Interfce action needs to be set externally
			if(GUIobjs[i].type==GUIObj::BUTTON)
			{
				assert(!env->hasComponents<UserInterface>(i));
				env->addComponents<UserInterface>(i, new UserInterface(std::bitset<UIstates>(1<<UserInterface::HIGHLIGHT|1<<UserInterface::CLICK|1<<UserInterface::PRESS), 
																	   &GUIobjs[i].action));
			}
		}
		if(env->hasComponents<Label>(i) && env->hasComponents<Transform>(i))
		{
			labels[i].label.setPosition(sf::Vector2f(trans[i].x+sprite[i].sprite.getLocalBounds().width/2, trans[i].y+sprite[i].sprite.getLocalBounds().height/2));
			//PRINT_DEBUG(cout<<i<<endl, LOW_DEBUG, GFXSYS);
			//sf::Font font;
			//	if(!font.loadFromFile("arial.ttf")) PRINT_DEBUG(cout<<"ERROR: FONT NOT FOUND!", LOW_DEBUG, GFXSYS);
			//	labels[i].label.setFont(font);
		}
	}
}

UISystem::~UISystem() {}


void UISystem::update(Environment* env)
{
	auto ui = env->get<UserInterface>();
	auto mouse = env->get<MouseControls>();

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
