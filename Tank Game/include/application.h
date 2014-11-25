#pragma once
#include <SFML/Graphics.hpp>
#include "core/memory.h"
#include "components/transform.h"
#include "components/texture_handle.h"

class Application
{
public:
	Application();
	~Application();

	int run();	

private:
	sf::RenderWindow* window;

	Environment<10, Transform, TextureHandle> main_env;
};
