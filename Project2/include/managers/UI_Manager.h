#pragma once
#include <functional>
#include "core/environment.h"
#include "../math/vector.h"

class UI_Manager
{
	public:
		UI_Manager();
		virtual ~UI_Manager();

		int CreateButton(Environment* _env, Vec2f _pos, std::function<void*()> _action, std::string _lable, std::string _name, bool _visible = true);
		int CreatePane(Environment* _env, Vec2f _pos, std::string _lable, std::string _name, bool _visible);
		void CreateMenu(Environment* _env, sf::RenderWindow* _win);
		
	private:
		enum MENU
		{
			MAIN_MENU = 0,
			OPTIONS_MENU,
			ABOUT_MENU
		};

		vector<unsigned int> mainMenu, optionsMenu, aboutMenu;
		short currMenu;
		bool* visible;
};

