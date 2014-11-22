#include <SFML/Graphics.hpp>
#include "core/framework.h"
using namespace sf;

// Let this be global for now, it's here for debugging objects. 
// Unless you can come up with a way to draw graphical debugs another way.
RenderWindow* window; 

int main()
{
	window = new RenderWindow(VideoMode(1080, 720), "Tank Game");
	delete window;

	return 0;
}
