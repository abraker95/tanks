#include "systems/render_system.h"
#include "Components.h"
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

void RenderSystem::update(Environment* env, sf::RenderWindow* _win, EntityManager* _entMgr, UI_Manager* _uiMgr, MapLoader* _mapLdr)
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

	for(unsigned i=0;i<env->maxEntities();i++)
	{
		if(env->hasComponents<ViewController>(i))
		{
			GameScene.setView(view_controller[i].view);
			for(unsigned k=0;k<_mapLdr->IDs.size();k++)
			{
				unsigned int ID = _mapLdr->IDs[k];
				if(env->hasComponents<VertexArray, Texture, Tilemap>(ID))
				{
					GameScene.draw(*vertex_array[ID].array, textures[ID].texture);
					break; // should only be one map
				}
			}

			for(unsigned k = 0; k<_entMgr->IDs.size(); k++)
			{
				// temporary fix
				unsigned int ID = _entMgr->IDs[k];
				if(env->hasComponents<Transform, Sprite, Texture>(ID)&&
					!env->hasComponents<GUIObj>(ID))
				{
					sf::Sprite& sprite = sprites[ID].sprite;
						sprite.setPosition(trans[ID].pos.x, trans[ID].pos.y);
						sprite.setRotation(trans[ID].rot);
						sprite.setTexture(*textures[ID].texture);
						GameScene.draw(sprite);
				}
			}
		}
	

		// blur the contents behind the menu
		if(sf::Shader::isAvailable())
		{
			if(env->getEntityName(i) == "ESC UI") /// \NOTE: If there is a memory error within this scope, check getEntityName for bugs
			{
				auto GUIobjs = env->get<GUIObj>();
				auto visible = env->get<StdComponent<bool>>();  /// \TODO: keep teack of this when breating other stdcomponent bools
				auto blur = env->get<StdComponent<sf::Shader>>();

				if(GUIobjs[i].type==GUIObj::VOID)
				{
					if(*visible[i].data && visible[i].name == "visible")
					{
						blur[i].data->setParameter("tex0", GameScene.getTexture());
						shader = blur[i].data;	
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
