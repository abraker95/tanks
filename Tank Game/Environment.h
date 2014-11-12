#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <vector>
#include "object.h"

class Environment
{
public:
	Environment();
	virtual ~Environment();

	void Loop(RenderWindow* _window);
	void Update(float _elapsedTime);
	void Render(RenderWindow* _window);
	void addObject(Object* _obj) { objects.push_back(_obj); }
	static Environment* getSingleton() { return singleton; }
	static void initSingleton();
	static void deinitSingleton();

private:
	//std::vector<Bullet*> bullets;
	std::vector<Object*> objects;
	static Environment* singleton;
};
