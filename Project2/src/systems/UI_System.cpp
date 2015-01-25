#include "Components.h"
#include "systems/UI_System.h"
#include "events.h"

UISystem::UISystem() {}

UISystem::~UISystem() {}


void UISystem::update(Environment* _env, Managers* managers)
{
	auto ui = _env->get<UserInterface>();

	if(*managers->UI_manager.visible == true)
	{
		if(managers->UI_manager.currMenu==UI_Manager::NO_MENU) *managers->UI_manager.visible = false;
		else									  *managers->UI_manager.visible = true;
		
		for(unsigned ID = 0; ID<_env->maxEntities(); ID++)
		{
			// \TODO: Should the press flag be still on if the cursor moves from the object while user is still
			//        holding the button or not?
			if(_env->hasComponents<UserInterface>(ID))
			{
				ui[ID].show = false;
				if(managers->UI_manager.isVisible(&ui[ID]))
				{
					ui[ID].show = true;

					// save the drag state if the button is pressed since it is going to be reset. 
					// This is needed for the logic handeling the case where the cursor goes fast
					// enough to escape the object's bounds
					bool drag   = ui[ID].state.test(UserInterface::DRAG),
						 toggle = ui[ID].state.test(UserInterface::TOGGLE),
						 focus  = ui[ID].state.test(UserInterface::FOCUS);

					ui[ID].state.reset();

					if(focus)
						ui[ID].state.set(UserInterface::FOCUS);
					
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
									ui[ID].state.set(UserInterface::CLICK);
									ui[ID].state.set(UserInterface::FOCUS);
									input_manager.mouseClickState.set(InputControls_Mgr::MOUSE::LEFT, false);
									ui[ID].action();
								}
							}
						}
					}
					else if(!ui[ID].cursorOnThis)
					{
						if(input_manager.mouseClickState.test(InputControls_Mgr::MOUSE::LEFT))
						{
							/// \TODO: Fix focus code
							if(ui[ID].enable.test(UserInterface::FOCUS))	ui[ID].state.reset(UserInterface::FOCUS);
						}
					}

					if(input_manager.mousePressState.test(InputControls_Mgr::MOUSE::LEFT)&&drag)
						ui[ID].state.set(UserInterface::DRAG);

					if(ui[ID].state.test(UserInterface::FOCUS))
						if(ui[ID].enable.test(UserInterface::KEY))
							if(input_manager.keyClickState.any())
								ui[ID].state.set(UserInterface::KEY);
				}
			}
		}
	}
	else if(*managers->UI_manager.visible == false)
	{
		if(managers->UI_manager.currMenu == UI_Manager::NO_MENU) *managers->UI_manager.visible = false;
		else					   				    *managers->UI_manager.visible = true;
	}

	auto gameOverEvent = _env->getEvents<GameOverEvent>();
	if(gameOverEvent.size()>0)
	{
		// display Win/Loss message
		/* Messages chossen at random:
		"Player X has been shot to death by player 2!"
		"Player X has been annhialited!"
		"Player X won a cookie!"
		"Player X just didn't have the guns!"
		"Player X swallowed a bullet!"
		"And then Player X took a bullet to the wheel"
		"Player X left this cruel world!"
		*/
		/*
		In case of a tie:
		"Player X and Player Y have died in a Mexican standoff!"
		"Player X and Player Y exchanged bullets!"
		"Player X and Player Y quit on life!"
		"Player X took Player Y down with it!"
		"Player X and Player Y got tied to the wrong game!"
		*/

		cout<<"GameOver"<<endl;
		managers->UI_manager.ShowGameOver();
	}

	auto newGameEvent = _env->getEvents<NewGameEvent>();
	if(newGameEvent.size()>0)
		managers->entity_manager.ResetGame(_env);
}
