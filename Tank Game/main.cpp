#include <SFML/Graphics.hpp>
#include "Object.h"
//#include "Keyboard.h"

using namespace sf;

int main()
{
	RenderWindow* window = new RenderWindow(VideoMode(1080, 720), "SFML works!");

	Object tank("Tank", 1);
    tank.setPosition(0, 0);

	while(window->isOpen())
	{
		Event event;
		while(window->pollEvent(event))
		{
 			if(event.type == Event::Closed)
				window->close();
		}

		window->clear();
		tank.Render(window);
		window->display();
	}

	delete window;
	return 0;
}
