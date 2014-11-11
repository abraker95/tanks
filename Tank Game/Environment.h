#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Tank.h"
#include "Bullet.h"

class Environment
{
public:
	Environment();
	virtual ~Environment();

	void Loop(RenderWindow* _window);
	void Update(float _elapsedTime);
	void Render(RenderWindow* _window);
private:
	Tank* player1;
	std::vector<Bullet*> bullets;
};
