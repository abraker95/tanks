#pragma once
#include <functional>
#include "core/environment.h"
#include "components/UserInterface.h"
#include "components/Label.h"
#include "../math/vector.h"

class UI_Manager
{
	friend class UISystem;

	enum MENU
	{
		NO_MENU = 0,
		MAIN_MENU,
		OPTIONS_MENU,
		NET_MENU,
		ABOUT_MENU,
		CHANGENAME_MENU,
		GAME_OVER
	};

	public:
		UI_Manager();
		virtual ~UI_Manager();

		void CreateMenu(Environment* _env, sf::RenderWindow* _win, bool& fullscreen);
		bool isVisible(UserInterface* _UI);
		void ShowGameOver();

	private:
		short currMenu;
		bool* visible;
		
		unsigned CreateButton(Environment* _env, Vec2f _pos, std::function<void*()> _action, std::string _lable, std::string _name, MENU _subMenu);
		unsigned CreatePane(Environment* _env, Vec2f _pos, std::string _lable, std::string _name, MENU _subMenu);
		unsigned CreateTextField(Environment* _env, Vec2f _pos, std::string _name, MENU _subMenu);

		void CreateMainSubMenu(Environment* _env);
		void CreateOptionsSubMenu(Environment* _env, sf::RenderWindow* _win, bool& fullscreen);
		void CreateAboutSubMenu(Environment* _env);
		void CreateGameOverSubMenu(Environment* _env);
		void CreateChangeNameSubMenu(Environment* _env);
		void CreateVoidSubMenu(Environment* _env);
		void CreateNetSubMenu(Environment* _env);
};

