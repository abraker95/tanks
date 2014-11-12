#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <vector>
#include "GameObject.h"

class Environment
{
public:
	Environment();
	virtual ~Environment();

	void Loop(RenderWindow* _window);
	void Update(float _elapsedTime);
	void Render(RenderWindow* _window);
	void addObject(GameObject* _obj) { objects.push_back(_obj); }
	static Environment* getSingleton() { return singleton; }
	static void initSingleton();
	static void deinitSingleton();

private:
	std::vector<GameObject*> objects;
	static Environment* singleton;

	void checkCollisions(GameObject* _obj);
};
