#include "Components.h"
#include "managers.h"
#include "systems/UI_System.h"
#include "events.h"

UISystem::UISystem() {}

/// \TODO: UI_Manager
UISystem::UISystem(Environment* env)
{
	// Factory based procedure for constructing GUIObjects
	auto GUIobjs = env->get<GUIObj>();
	auto sprite = env->get<Sprite>();
	auto trans = env->get<Transform>();
	auto labels = env->get<Label>();

	for(unsigned i = 0; i<env->maxEntities(); i++)
	{
		// \TODO: Add color component for customization
		if(env->hasComponents<GUIObj>(i))
		{
			if(GUIobjs[i].type==GUIObj::VOID)
			{
				assert(!env->hasComponents<UserInterface>(i));
				env->addComponents(i, new UserInterface(std::bitset<UIstates>(), &GUIobjs[i].action));
				//env->removeComponents<GUIObj>(i);
			}
		}
	}
}

UISystem::~UISystem() {}


void UISystem::update(Environment* env, EntityManager* _entMgr, TextureManager* _texMgr)
{
	auto ui = env->get<UserInterface>();
	auto GUIobjs = env->get<GUIObj>();

	for(unsigned i = 0; i<env->maxEntities(); i++)
	{
		// \TODO: Should the press flag be still on if the cursor moves from the object while user is still
		//        holding the button or not?
		if(env->hasComponents<UserInterface, GUIObj>(i))
		{
			if(GUIobjs[i].type==GUIObj::VOID)
			{
				if(input_manager.keyClickState.test(sf::Keyboard::Escape))
				{
					auto visible = env->get<StdComponent<bool>>()[i].data;
					if(ui[i].action) (*ui[i].action)(); 
					env->emit(new MenuEvent(*visible));
				}		
			}

			if(ui[i].show)
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
					if(input_manager.mousePressState.test(InputControls_Mgr::MOUSE::LEFT))
					{
						if(ui[i].enable.test(UserInterface::TOGGLE) && !toggle) ui[i].state.set(UserInterface::TOGGLE);
						if(ui[i].enable.test(UserInterface::PRESS))				ui[i].state.set(UserInterface::PRESS);
						if(ui[i].enable.test(UserInterface::DRAG))				ui[i].state.set(UserInterface::DRAG);
						if(input_manager.mouseClickState.test(InputControls_Mgr::MOUSE::LEFT))
						{
							input_manager.mouseClickState.set(InputControls_Mgr::MOUSE::LEFT, false);
							if(ui[i].action) (*ui[i].action)();
						}
					}
				}

				if(input_manager.mousePressState.test(InputControls_Mgr::MOUSE::LEFT) && drag)
					ui[i].state.set(UserInterface::DRAG);
			}
		}
	}

	auto newGameEvent = env->getEvents<NewGameEvent>();
	if(newGameEvent.size()>0)
		_entMgr->ResetGame(env, _texMgr);
}
