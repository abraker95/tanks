#pragma once
#include <functional>
#include "core/environment.h"
#include "../math/vector.h"

class UI_Manager
{
	public:
		UI_Manager();
		virtual ~UI_Manager();

		void CreateButton(Environment* _env, Vec2f _pos, std::function<void*()> _action, std::string _lable, std::string _name);
		void CreateMenu(Environment* _env, sf::RenderWindow* _win);
		//void CreateVoid(std::string _name);
};

