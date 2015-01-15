#include "Components.h"
#include "managers.h"
#include "systems/UI_System.h"
#include "events.h"

UISystem::UISystem() {}

UISystem::~UISystem() {}


void UISystem::update(Environment* _env, UI_Manager* _uiMgr, EntityManager* _entMgr, TextureManager* _texMgr)
{
	auto ui = _env->get<UserInterface>();

	if(*_uiMgr->visible == true)
	{
		if(_uiMgr->currMenu==UI_Manager::NO_MENU) *_uiMgr->visible = false;
		else									  *_uiMgr->visible = true;
		
		for(unsigned ID = 0; ID<_env->maxEntities(); ID++)
		{
			// \TODO: Should the press flag be still on if the cursor moves from the object while user is still
			//        holding the button or not?
			if(_env->hasComponents<UserInterface>(ID))
			{
				ui[ID].show = false;
				if(_uiMgr->isVisible(&ui[ID]))
				{
					ui[ID].show = true;

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
	else if(*_uiMgr->visible == false)
	{
		if(_uiMgr->currMenu == UI_Manager::NO_MENU) *_uiMgr->visible = false;
		else					   				    *_uiMgr->visible = true;
	}

	auto scoreEvent = _env->getEvents<ScoreEvent>();
	if(scoreEvent.size()>0)
	{
		// display Win/Loss message
		/* Messages chossen at random:
		"Player X has been shot to death by player 2!"
		"Player X has been annhialited!"
		"Player X won a cookie!"
		"Player X just didn't have the guns!"
		"Player X swallowed a bullet!"
		"And then Player X took a bullet to the wheel"
		"OMG! You killed Player X! What's wrong with you?"
		"Player X left this cruel world!"
		*/
		/*
		In case of a tie:
		"Player X and Player Y have died in a Mexican standoff!"
		"Player X and Player Y exchanged bullets!"
		"Player X and Player Y quit on life!"
		"Player X took Player Y down with it!"
		"Player X and Player Y got tied to the wrong game!"
		"At least nobody won nor lost..."
		*/

		cout<<"GameOver"<<endl;
		_uiMgr->ShowGameOver();
	}

	auto newGameEvent = _env->getEvents<NewGameEvent>();
	if(newGameEvent.size()>0)
		_entMgr->ResetGame(_env, _texMgr, newGameEvent[0].resetScores);
}
