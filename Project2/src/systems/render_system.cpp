#include "Components.h"
#include "systems/render_system.h"
#include "utils.h"
#include "events.h"

RenderSystem::RenderSystem(sf::RenderWindow* _win)
{
	GameScene.create(_win->getSize().x, _win->getSize().y);
	UIScene.create(_win->getSize().x, _win->getSize().y);
	shader = nullptr;
	//fullscreen = new bool(false);
}

RenderSystem::~RenderSystem()
{
	//delete fullscreen;
}

void RenderSystem::update(Environment* env, sf::RenderWindow* _win)
{
	auto sprites = env->get<Sprite>();
	auto trans = env->get<Transform>();
	auto textures = env->get<Texture>();
	auto vertex_array = env->get<VertexArray>();
	auto view_controller = env->get<ViewController>();

	auto WindowMode = env->getEvents<WindowModeEvent>();
	if(WindowMode.size()>0)
		fullscreen = *WindowMode[0].fullscreen;

	if(prevFullscreen != fullscreen)
	{
		GameScene.create(_win->getSize().x, _win->getSize().y);
		UIScene.create(_win->getSize().x, _win->getSize().y);
	}
	prevFullscreen = fullscreen;

	//if(GameScene.getSize() != _win->getSize())
	env->emit(new WindowModeEvent(&fullscreen));

	for(unsigned k=0;k<env->maxEntities();k++)
	{
		if(env->hasComponents<ViewController>(k))
		{
			GameScene.setView(view_controller[k].view);
			for(unsigned i=0;i<env->maxEntities();i++)
			{
				if(env->hasComponents<VertexArray, Texture, Tilemap>(i))
				{
					GameScene.draw(*vertex_array[i].array, textures[i].texture);
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
						sprite.setPosition(trans[i].pos.x, trans[i].pos.y);
						sprite.setRotation(trans[i].rot);
						sprite.setTexture(*textures[i].texture);
						GameScene.draw(sprite);
				}
			}
		}
	}

	for(unsigned i=0;i<env->maxEntities();i++)
	{
		// blur the contents behind the menu
		if(sf::Shader::isAvailable())
		{
			if(env->getEntityName(i) == "ESC UI") /// \NOTE: If there is a memory error within this scope, check getEntityName for bugs
			{
				auto GUIobjs = env->get<GUIObj>();
				auto visible = env->get<StdComponent<bool>>();  /// \TODO: keep teack of this when breating other stdcomponent bools
				auto shaderObj = env->get<StdComponent<sf::Texture>>();
				auto blur = env->get<StdComponent<sf::Shader>>();

				if(GUIobjs[i].type==GUIObj::VOID)
				{
					if(*visible[i].data && visible[i].name == "visible")
					{
						if(shaderObj[i].data == nullptr)
						{
							//PRINT_DEBUG(std::cout<<"Texture:"<<shaderObj[i].data<<std::endl, HI_DEBUG, GFXSYS);
							shaderObj[i].data = new sf::Texture();
						//	PRINT_DEBUG(std::cout<<"Texture: "<<shaderObj[i].data<<"  Source: "<<env->get<StdComponent<sf::Texture>>()[i].data<<std::endl, HI_DEBUG, GFXSYS);
							shaderObj[i].data->create(_win->getSize().x, _win->getSize().y);
							shaderObj[i].set((sf::Texture*)&GameScene.getTexture());
						}
						
						bool validShader = blur[i].data != nullptr;
						if(blur[i].data == nullptr)
						{
						//	PRINT_DEBUG(std::cout<<"shader"<<std::endl, HI_DEBUG, GFXSYS);
							blur[i].data = new sf::Shader();
						//	PRINT_DEBUG(std::cout<<"Shader: "<<shaderObj[i].data<<"  Source: "<<env->get<StdComponent<sf::Shader>>()[i].data<<std::endl, HI_DEBUG, GFXSYS);
							validShader = blur[i].data->loadFromFile("res/blur.vert", "res/blur.frag");
						}
						
						if(validShader)
						{
							blur[i].data->setParameter("tex0", *shaderObj[i].data);
							blur[i].data->setParameter("intensity", 0.004f);

							shader = blur[i].data;
						//	GameScene.draw(sf::Sprite(*shaderObj[i].data), blur[i].data);
						}
						else PRINT_DEBUG(std::cout<<"ERROR: Could not load shader"<<std::endl, HI_DEBUG, GFXSYS);	
					}
					else shader = nullptr;
				}
			}
		}

		/// \TODO: have the button's dimentions update when window size changes
		if(env->hasComponents<Transform, UserInterface, Label, StdComponent<sf::RectangleShape>>(i))
		{
			auto ui = env->get<UserInterface>();
			auto GUIobjs = env->get<GUIObj>();

			if(ui[i].show)
			{
				if(GUIobjs[i].type==GUIObj::BUTTON)
				{
					auto labels = env->get<Label>();
					auto button = env->get<StdComponent<sf::RectangleShape>>();

					const float margin = 50;
					sf::FloatRect dim = sf::FloatRect(trans[i].pos.x, trans[i].pos.y,
						labels[i].label.getLocalBounds().width+margin, labels[i].label.getLocalBounds().height+margin);

						button[i].data->setSize(sf::Vector2f(dim.width, dim.height));
						button[i].data->setFillColor(sf::Color(50, 50, 100, 255));
					UIScene.draw(*button[i].data);

					Vec2i pos = sf::Mouse::getPosition(*_win);

					//	PRINT_DEBUG(std::cout<<" X pos view: "<<trans[i].x<<" X pos screen: "<<pos.x<<std::endl, HI_DEBUG, GFXSYS);
					//	PRINT_DEBUG(std::cout<<" X range: "<<trans[i].x<<" "<<pos.x-window->getPosition().x<<" "<<bounds.width+trans[i].x<<std::endl, HI_DEBUG, GFXSYS);
					//	PRINT_DEBUG(std::cout<<" Y range: "<<trans[i].y<<" "<<sf::Mouse::getPosition().y-window->getPosition().y<<" "<<bounds.height+trans[i].y<<std::endl<<endl, HI_DEBUG, GFXSYS);

					if(BTWN(dim.left, pos.x, dim.width+dim.left)&&BTWN(dim.top, pos.y, dim.height+dim.top))
						ui[i].cursorOnThis = true;
					else
						ui[i].cursorOnThis = false;

					if(ui[i].state.test(UserInterface::PRESS))
					{
						button[i].data->setFillColor(sf::Color(50, 50, 50, 50));
						UIScene.draw(*button[i].data);
					}
					else if(ui[i].state.test(UserInterface::HIGHLIGHT))  // Just because the cursor is on the UI, doesn't mean it will always highlight. e.i.: Highlight is not enabled
					{
						button[i].data->setFillColor(sf::Color(200, 200, 200, 50));
						UIScene.draw(*button[i].data);
					}

					if(ui[i].state.test(UserInterface::DRAG))
					{
						trans[i].pos = sf::Mouse::getPosition();
					}

					UIScene.draw(labels[i].label);
				}
				else if(GUIobjs[i].type==GUIObj::PANE)
				{
					auto labels = env->get<Label>();
					auto button = env->get<StdComponent<sf::RectangleShape>>();

					const float margin = 50;
					sf::FloatRect dim = sf::FloatRect(trans[i].pos.x, trans[i].pos.y,
						labels[i].label.getLocalBounds().width+margin, labels[i].label.getLocalBounds().height+margin);

					button[i].data->setSize(sf::Vector2f(dim.width, dim.height));
					button[i].data->setOutlineColor(sf::Color(50, 50, 100, 255));
					button[i].data->setFillColor(sf::Color(0, 0, 0, 0));
					button[i].data->setOutlineThickness(2.0f);

					UIScene.draw(*button[i].data);
					UIScene.draw(labels[i].label);
				}
			}
		}

		if(env->hasComponents<Transform, Label>(i))
		{
			if(env->getEntityName(i)=="CPU")
			{
				if(*env->get<StdComponent<bool>>()[i].data)
				{
					auto labels = env->get<Label>();
					UIScene.draw(labels[i].label);
				}
				
			}
			
		}
	}

	_win->draw(sf::Sprite(GameScene.getTexture()), shader);						     GameScene.clear(sf::Color(0, 0, 0, 0)); GameScene.display();
	_win->draw(sf::Sprite(UIScene.getTexture()), sf::RenderStates(sf::BlendAlpha));  UIScene.clear(sf::Color(0, 0, 0, 0));   UIScene.display();
	_win->display();
}
