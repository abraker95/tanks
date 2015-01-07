#include "Components.h"
#include "managers.h"
#include "systems/UI_System.h"
#include "events.h"

UISystem::UISystem() {}

UISystem::~UISystem() {}


void UISystem::update(Environment* _env, UI_Manager* _uiMgr, EntityManager* _entMgr, TextureManager* _texMgr)
{
	auto ui = _env->get<UserInterface>();
	auto GUIobjs = _env->get<GUIObj>();

	if(*_uiMgr->visible)
	{
		for(unsigned i = 0; i<_uiMgr->IDs.size(); i++)
		{
			unsigned int ID = _uiMgr->IDs[i];

			// \TODO: Should the press flag be still on if the cursor moves from the object while user is still
			//        holding the button or not?
			if(_env->hasComponents<UserInterface, GUIObj>(ID))
			{
				if(ui[ID].show)
				{
					// save the drag state if the button is pressed since it is going to be reset. 
					// This is needed for the logic handeling the case where the cursor goes fast
					// enough to escape the object's bounds
					bool drag = ui[ID].enable.test(UserInterface::DRAG),
						toggle = ui[ID].enable.test(UserInterface::TOGGLE);

					ui[ID].state.reset();
					if(ui[ID].cursorOnThis)
					{
						if(ui[ID].enable.test(UserInterface::HIGHLIGHT)) ui[ID].state.set(UserInterface::HIGHLIGHT);
						if(input_manager.mousePressState.test(InputControls_Mgr::MOUSE::LEFT))
						{
							if(ui[ID].enable.test(UserInterface::TOGGLE)&&!toggle) ui[ID].state.set(UserInterface::TOGGLE);
							if(ui[ID].enable.test(UserInterface::PRESS))		   ui[ID].state.set(UserInterface::PRESS);
							if(ui[ID].enable.test(UserInterface::DRAG))			   ui[ID].state.set(UserInterface::DRAG);
							if(ui[ID].enable.test(UserInterface::CLICK))
							{
								if(input_manager.mouseClickState.test(InputControls_Mgr::MOUSE::LEFT))
								{
									input_manager.mouseClickState.set(InputControls_Mgr::MOUSE::LEFT, false);
									if(ui[ID].action) (*ui[ID].action)();
								}
							}
						}
					}

					if(input_manager.mousePressState.test(InputControls_Mgr::MOUSE::LEFT)&&drag)
						ui[ID].state.set(UserInterface::DRAG);
				}
			}
		}
	}

	auto newGameEvent = _env->getEvents<NewGameEvent>();
	if(newGameEvent.size()>0)
		_entMgr->ResetGame(_env, _texMgr);
}
