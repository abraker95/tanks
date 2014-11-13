#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <vector>
#include "GameObject.h"
#include "Tilemap.h"

class Environment
{
public:
	Environment();
	virtual ~Environment();

	void Loop(RenderWindow* _window);
	void Update(float _elapsedTime);
	void Render(RenderWindow* _window);
	void setTilemap(Tilemap* _tilemap) { tilemap = _tilemap; }
	void addObject(GameObject* _obj) { objects.push_back(_obj); }
	const std::vector<GameObject*>& getObjects() const { return objects; } // read only
	static Environment* getSingleton() { return singleton; }
	static void initSingleton();
	static void deinitSingleton();

private:
	std::vector<GameObject*> objects;
	Tilemap* tilemap;
	static Environment* singleton;

	void checkCollisions(GameObject* _obj);
};
