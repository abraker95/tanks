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

void RenderSystem::update(Environment* env, sf::RenderWindow* _win, EntityManager* _entMgr, UI_Manager* _uiMgr, CPUManager* _cpuMgr, MapLoader* _mapLdr)
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

	for(unsigned i = 0; i<_entMgr->IDs.size(); i++)
	{
		unsigned int entID = _entMgr->IDs[i];
		if(env->hasComponents<ViewController>(entID))
		{
			GameScene.setView(view_controller[entID].view);
			for(unsigned k = 0; k<_mapLdr->IDs.size(); k++)
			{
				unsigned int mapID = _mapLdr->IDs[k];
				if(env->hasComponents<VertexArray, Texture, Tilemap>(mapID))
				{
					GameScene.draw(*vertex_array[mapID].array, textures[mapID].texture);
					break; // should only be one map
				}
			}

			// temporary fix
			for(unsigned k = 0; k<_entMgr->IDs.size(); k++)
			{
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
			for(unsigned k = 0; k<_mapLdr->IDs.size(); k++)
			{
				unsigned int ID = _mapLdr->IDs[k];
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
	}

	for(unsigned i = 0; i<_uiMgr->IDs.size(); i++)
	{
		unsigned int ID = _uiMgr->IDs[i];
		auto GUIobjs = env->get<GUIObj>();

		// blur the contents behind the menu
		if(sf::Shader::isAvailable())
		{
			if(env->getEntityName(ID)=="ESC UI") /// \NOTE: If there is a memory error within this scope, check getEntityName for bugs
			{
				auto visible = env->get<StdComponent<bool>>();  /// \TODO: keep teack of this when breating other stdcomponent bools
				auto blur = env->get<StdComponent<sf::Shader>>();

				if(GUIobjs[ID].type==GUIObj::VOID)
				{
					if(*visible[ID].data && visible[ID].name=="visible")
					{
						blur[ID].data->setParameter("tex0", GameScene.getTexture());
						shader = blur[ID].data;
					}
					else shader = nullptr;
				}
			}
		}

		/// \TODO: have the button's dimentions update when window size changes
		if(env->hasComponents<Transform, UserInterface, Label, StdComponent<sf::RectangleShape>>(ID))
		{
			auto ui = env->get<UserInterface>();

			if(ui[ID].show)
			{
				if(GUIobjs[ID].type==GUIObj::BUTTON)
				{
					auto labels = env->get<Label>();
					auto button = env->get<StdComponent<sf::RectangleShape>>();

					const float margin = 50;
					sf::FloatRect dim = sf::FloatRect(trans[ID].pos.x, trans[ID].pos.y,
						labels[ID].label.getLocalBounds().width+margin, labels[ID].label.getLocalBounds().height+margin);

					button[ID].data->setSize(sf::Vector2f(dim.width, dim.height));
					button[ID].data->setFillColor(sf::Color(50, 50, 100, 255));
					UIScene.draw(*button[ID].data);

					Vec2i pos = sf::Mouse::getPosition(*_win);

					//	PRINT_DEBUG(std::cout<<" X pos view: "<<trans[i].x<<" X pos screen: "<<pos.x<<std::endl, HI_DEBUG, GFXSYS);
					//	PRINT_DEBUG(std::cout<<" X range: "<<trans[i].x<<" "<<pos.x-window->getPosition().x<<" "<<bounds.width+trans[i].x<<std::endl, HI_DEBUG, GFXSYS);
					//	PRINT_DEBUG(std::cout<<" Y range: "<<trans[i].y<<" "<<sf::Mouse::getPosition().y-window->getPosition().y<<" "<<bounds.height+trans[i].y<<std::endl<<endl, HI_DEBUG, GFXSYS);

					if(BTWN(dim.left, pos.x, dim.width+dim.left)&&BTWN(dim.top, pos.y, dim.height+dim.top))
						ui[ID].cursorOnThis = true;
					else
						ui[ID].cursorOnThis = false;
					
					if(ui[ID].state.test(UserInterface::PRESS))
					{
						button[ID].data->setFillColor(sf::Color(50, 50, 50, 50));
						UIScene.draw(*button[ID].data);
					}
					else if(ui[ID].state.test(UserInterface::HIGHLIGHT))  // Just because the cursor is on the UI, doesn't mean it will always highlight. e.i.: Highlight is not enabled
					{
						button[ID].data->setFillColor(sf::Color(200, 200, 200, 50));
						UIScene.draw(*button[ID].data);
					}

					if(ui[ID].state.test(UserInterface::DRAG))
					{
						trans[ID].pos = sf::Mouse::getPosition();
					}

					UIScene.draw(labels[ID].label);
				}
				else if(GUIobjs[ID].type==GUIObj::PANE)
				{
					auto labels = env->get<Label>();
					auto button = env->get<StdComponent<sf::RectangleShape>>();

					const float margin = 50;
					sf::FloatRect dim = sf::FloatRect(trans[ID].pos.x, trans[ID].pos.y,
						labels[ID].label.getLocalBounds().width+margin, labels[ID].label.getLocalBounds().height+margin);

					button[ID].data->setSize(sf::Vector2f(dim.width, dim.height));
					button[ID].data->setOutlineColor(sf::Color(50, 50, 100, 255));
					button[ID].data->setFillColor(sf::Color(0, 0, 0, 0));
					button[ID].data->setOutlineThickness(2.0f);

					UIScene.draw(*button[ID].data);
					UIScene.draw(labels[ID].label);
				}
			}
		}
	}


	for(unsigned i = 0; i<_cpuMgr->IDs.size(); i++)
	{
		unsigned int ID = _cpuMgr->IDs[i];
		if(env->hasComponents<Transform, Label>(ID))
		{
			if(env->getEntityName(ID)=="CPU")
			{
				if(*env->get<StdComponent<bool>>()[ID].data)
				{
					auto labels = env->get<Label>();
					UIScene.draw(labels[ID].label);
				}
			}
			
		}
	}

	_win->draw(sf::Sprite(GameScene.getTexture()), shader);						     GameScene.clear(sf::Color(0, 0, 0, 0)); GameScene.display();
	_win->draw(sf::Sprite(UIScene.getTexture()), sf::RenderStates(sf::BlendAlpha));  UIScene.clear(sf::Color(0, 0, 0, 0));   UIScene.display();
	_win->display();
}
