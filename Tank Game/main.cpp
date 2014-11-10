#include <SFML/Graphics.hpp>
#include "Object.h"
#include "Tank.h"
//#include "Keyboard.h"

using namespace sf;

int main()
{
	RenderWindow* window = new RenderWindow(VideoMode(1080, 720), "SFML works!");

	Tank tank;
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
			tank.Update(window, 0.001);
		window->display();
	}

	delete window;
	return 0;
}
