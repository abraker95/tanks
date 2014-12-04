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
	Sprite* sprites = env->get<Sprite>();
	Texture* textures = env->get<Texture>();
	VertexArray* vertex_array = env->get<VertexArray>();
	ViewController* view_controller = env->get<ViewController>();

	/*
	window->clear();
	sf::View view = sf::View(sf::Vector2f(0, 0), (sf::Vector2f)window->getSize());
		view.setCenter(sf::Vector2f(window->getSize().x/2, window->getSize().y/2));
		window->setView(view);
	*/

	for(unsigned k=0;k<env->maxEntities();k++)
	{

		if(env->hasComponents<ViewController>(k))
		{
			window->setView(view_controller[k].view);
			for(unsigned i=0;env->maxEntities();i++)
			{
				if(env->hasComponents<VertexArray, Texture, Tilemap>(i))
				{
					window->draw(vertex_array[i].vertices, textures[i].texture);
					break; // should only be one map
				}
			}

			for(unsigned i=0;i<env->maxEntities();i++)
			{
				// temporary fix
				if(env->hasComponents<Transform, Sprite, Texture>(i) &&
					!env->hasComponents<GUIObj>(i))
				{
					sf::Sprite& sprite = sprites[i].sprite;
					sprite.setPosition(trans[i].x, trans[i].y);
					sprite.setRotation(trans[i].rot);
					sprite.setTexture(*textures[i].texture);
					window->draw(sprite);
				}
			}
		}
	}

	// reset default view for UI, so that
	// the buttons don't move with the camera
	window->setView(window->getDefaultView());

	for(unsigned i=0;i<env->maxEntities();i++)
	{
		// temporary fix
		if(env->hasComponents<Transform, Sprite, Texture, GUIObj>(i))
		{
			sf::Sprite& sprite = sprites[i].sprite;
			sprite.setPosition(trans[i].x, trans[i].y);
			sprite.setRotation(trans[i].rot);
			sprite.setTexture(*textures[i].texture);
			window->draw(sprite);
		}
	}
	
	for(unsigned i=0;i<env->maxEntities();i++)
	{
		if(env->hasComponents<Transform, UserInterface, MouseControls>(i))
		{
			UserInterface* ui = env->get<UserInterface>();
			MouseControls* mouse = env->get<MouseControls>();
			sf::FloatRect bounds = sprites[i].sprite.getLocalBounds();
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
				sf::RectangleShape rectangle;
					rectangle.setPosition(trans[i].x, trans[i].y);
					rectangle.setSize(sf::Vector2f(bounds.width, bounds.height));
					rectangle.setFillColor(sf::Color(50, 50, 50, 50));
					window->draw(rectangle);
			}
			else if(ui[i].state.test(UserInterface::HIGHLIGHT))  // Just because the cursor is on the UI, doesn't mean it will always highlight. e.i.: Highlight is not enabled
			{
				sf::RectangleShape rectangle;
					rectangle.setPosition(trans[i].x, trans[i].y);
					rectangle.setSize(sf::Vector2f(bounds.width, bounds.height));
					rectangle.setFillColor(sf::Color(200, 200, 200, 50));
					window->draw(rectangle);
			}

			if(ui[i].state.test(UserInterface::DRAG))
			{
				trans[i].x = sf::Mouse::getPosition().x;
				trans[i].y = sf::Mouse::getPosition().y;
			}
		}
	}

	window->display();
}
