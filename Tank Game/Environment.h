#pragma once
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include "GameObject.h"
#include "VectorLN.h"
#include "Tilemap.h"
#include "Camera.h"
#include "EventMsgr.h"

class Environment
{
public:
	Environment();
	virtual ~Environment();

	void Loop(RenderWindow* _window);
	void Update(float _elapsedTime);
	void Render(RenderWindow* _window);
	void addObject(GameObject* _obj);
	const std::vector<GameObject*>& getObjects() const; // read only
	static Environment* getSingleton() { return singleton; }
	static void initSingleton();
	static void deinitSingleton();

private:
	static Environment* singleton;
	GameScene* gameScene;

	void checkCollisions(GameObject* _obj);
};
