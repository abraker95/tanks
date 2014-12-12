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
				env->addComponents(i, new UserInterface(std::bitset<UIstates>(1<<UserInterface::HIGHLIGHT|1<<UserInterface::CLICK|1<<UserInterface::PRESS), 
													   &GUIobjs[i].action),
								      new StdComponent<sf::RectangleShape>(new sf::RectangleShape()),
									  new MouseControls()
							       );

				if(labels[i].font.loadFromFile("arial.ttf")) cout<<"ERROR: FONT NOT FOUND"<<endl;
				labels[i].label.setFont(labels[i].font);

				int charSize = labels[i].label.getCharacterSize();
				int verticalFix = labels[i].font.getLineSpacing(charSize);
				labels[i].label.setPosition(sf::Vector2f(trans[i].x+25, trans[i].y+25-verticalFix));
				
				
				auto button = env->get<StdComponent<sf::RectangleShape>>();
					button[i].data->setPosition(trans[i].x, trans[i].y);
					button[i].data->setSize(sf::Vector2f(labels[i].label.getLocalBounds().width+50, labels[i].label.getLocalBounds().height+50));
			}
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
