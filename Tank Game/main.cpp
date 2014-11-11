#include <SFML/Graphics.hpp>
#include "Environment.h"
//#include "Keyboard.h"

using namespace sf;

int main()
{
	RenderWindow* window = new RenderWindow(VideoMode(1080, 720), "SFML works!");
	Environment* env = new Environment();

	env->Loop(window);

	delete env;
	delete window;

	return 0;
}
