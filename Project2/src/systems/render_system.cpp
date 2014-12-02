#include "Components.h"
#include "systems/render_system.h"
#include "utils.h"

RenderSystem::RenderSystem()
{

}

RenderSystem::~RenderSystem()
{
}

void RenderSystem::update(Environment* env, sf::RenderWindow* window)
{
	Transform* trans = env->get<Transform>();
	RenderProperties* props = env->get<RenderProperties>();

	window->clear();
	sf::View view = sf::View(sf::Vector2f(0, 0), (sf::Vector2f)window->getSize());
		view.setCenter(sf::Vector2f(window->getSize().x/2, window->getSize().y/2));
		window->setView(view);


	for(unsigned i=0;i<env->maxEntities();i++)
	{
		if(env->hasComponents<Transform, RenderProperties>(i))
		{
			sf::Sprite& sprite = props[i].sprite;
				sprite.setPosition(trans[i].x, trans[i].y);
				sprite.setRotation(trans[i].rot);
				window->draw(sprite);
		}

		if(env->hasComponents<Transform, UserInterface, MouseControls, RenderProperties>(i))
		{
			/// \TODO: Finish button highlighting
			UserInterface* ui = env->get<UserInterface>();
			MouseControls* mouse = env->get<MouseControls>();
			sf::FloatRect bounds = props[i].sprite.getLocalBounds();
			sf::Vector2f pos = window->mapPixelToCoords(sf::Mouse::getPosition(), window->getView());
			const int titleBar = 32;

		//	PRINT_DEBUG(std::cout<<" X pos view: "<<trans[i].x<<" X pos screen: "<<pos.x<<std::endl, HI_DEBUG, GFXSYS);
		//	PRINT_DEBUG(std::cout<<" X range: "<<trans[i].x<<" "<<pos.x-window->getPosition().x<<" "<<bounds.width+trans[i].x<<std::endl, HI_DEBUG, GFXSYS);
		//	PRINT_DEBUG(std::cout<<" Y range: "<<trans[i].y<<" "<<sf::Mouse::getPosition().y-window->getPosition().y<<" "<<bounds.height+trans[i].y<<std::endl<<endl, HI_DEBUG, GFXSYS);
			
			if(BTWN(trans[i].x, pos.x-window->getPosition().x, bounds.width+trans[i].x)
			&& BTWN(trans[i].y+titleBar, pos.y-window->getPosition().y, bounds.height+trans[i].y+titleBar))
				ui[i].cursorOnThis = true;
			else
				ui[i].cursorOnThis = false;
			
			if(ui[i].state.test(UserInterface::PRESS))
			{
				//	PRINT_DEBUG(std::cout<<"draw"<<std::endl, MED_DEBUG, GFXSYS);
				sf::RectangleShape rectangle;
				rectangle.setPosition(trans[i].x, trans[i].y);
				rectangle.setSize(sf::Vector2f(bounds.width, bounds.height));
				rectangle.setFillColor(sf::Color(50, 50, 50, 50));
				window->draw(rectangle);
			}
			else if(ui[i].state.test(UserInterface::HIGHLIGHT))  // Just because the cursor is on the UI, doesn't mean it will always highlight. e.i.: Highlight is not enabled
			{
			//	PRINT_DEBUG(std::cout<<"draw"<<std::endl, MED_DEBUG, GFXSYS);
				sf::RectangleShape rectangle;
				rectangle.setPosition(trans[i].x, trans[i].y);
					rectangle.setSize(sf::Vector2f(bounds.width, bounds.height));
					rectangle.setFillColor(sf::Color(200, 200, 200, 50));
					window->draw(rectangle);
			}
		}
	}

	window->display();
}
