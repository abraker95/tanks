#include <SFML/Graphics.hpp>
#include "Tank.h"
#include "Mine.h"
#include "Camera.h"
#include "Tilemap.h"
#include "Environment.h"
#include "GameScene.h"
using namespace sf;

// Let this be global for now, it's here for debugging objects. 
// Unless you can come up with a way to draw graphical debugs another way.
RenderWindow* window; 

int main()
{
	
	window = new RenderWindow(VideoMode(1080, 720), "Tank Game");

	Environment::initSingleton();
	Environment* env = Environment::getSingleton();

	env->Loop(window);

	Environment::deinitSingleton();
	delete window;

	return 0;
}
