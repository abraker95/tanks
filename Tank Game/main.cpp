#include <SFML/Graphics.hpp>
#include "Object.h"
//#include "Keyboard.h"

using namespace sf;

int main()
{
	RenderWindow* window = new RenderWindow(VideoMode(1080, 720), "SFML works!");
	//CircleShape shape(100.f);
	//shape.setFillColor(Color::Green);

	Object tank(window, "Tank", 1);
	tank.setPos(0, 0);

	while(window->isOpen())
	{
		Event event;
		while(window->pollEvent(event))
		{
 			if(event.type == Event::Closed)
				window->close();
		}

		window->clear();
		tank.Render();
		window->display();
	}

	delete window;
	return 0;
}
