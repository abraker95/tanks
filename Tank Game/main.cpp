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

	Clock clock;
	while(window->isOpen())
	{
		Event event;
		while(window->pollEvent(event))
		{
 			if(event.type == Event::Closed)
				window->close();
			if(event.type == Event::KeyPressed || event.type == Event::KeyReleased)
			{
				/// \TODO: Move this to the Environment class
				tank.UpdateUserInput();
			}
		}

		window->clear();
			Time elapsed = clock.restart();
			tank.Update(window, elapsed.asSeconds());
		window->display();
	}

	delete window;
	return 0;
}
