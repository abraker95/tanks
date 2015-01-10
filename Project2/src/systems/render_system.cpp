#include "systems/render_system.h"
#include "Components.h"
#include "utils.h"
#include "events.h"

RenderSystem::RenderSystem(sf::RenderWindow* _win)
{
	GameScene.create(_win->getSize().x, _win->getSize().y);
	UIScene.create(_win->getSize().x, _win->getSize().y);
	shader = nullptr;
}

RenderSystem::~RenderSystem()
{}

void RenderSystem::update(Environment* _mainEnv, Environment* _uiEnv, Environment* _gameEnv, sf::RenderWindow* _win, CPUManager* _cpuMgr)
{
	auto sprites = _gameEnv->get<Sprite>();
	auto Gametrans = _gameEnv->get<Transform>();
	auto uiTrans = _uiEnv->get<Transform>();
	auto textures = _gameEnv->get<Texture>();
	auto vertex_array = _gameEnv->get<VertexArray>();
	auto view_controller = _gameEnv->get<ViewController>();

	auto WindowMode = _mainEnv->getEvents<WindowModeEvent>();
	if(WindowMode.size()>0)
		fullscreen = *WindowMode[0].fullscreen;

	if(prevFullscreen != fullscreen)
	{
		GameScene.create(_win->getSize().x, _win->getSize().y);
		UIScene.create(_win->getSize().x, _win->getSize().y);
	}
	prevFullscreen = fullscreen;

	//if(GameScene.getSize() != _win->getSize())
	_mainEnv->emit(new WindowModeEvent(&fullscreen));

	for(unsigned viewID = 0; viewID<_gameEnv->maxEntities(); viewID++)
	{
		if(_gameEnv->hasComponents<ViewController>(viewID))
		{
			GameScene.setView(view_controller[viewID].view);
			for(unsigned mapID = 0; mapID<_gameEnv->maxEntities(); mapID++)
			{
				if(_gameEnv->hasComponents<VertexArray, Texture, Tilemap>(mapID))
				{
					GameScene.draw(*vertex_array[mapID].array, textures[mapID].texture);
					break; // should only be one map
				}
			}

			// temporary fix
			for(unsigned spriteID = 0; spriteID<_gameEnv->maxEntities(); spriteID++)
			{
				if(_gameEnv->hasComponents<Transform, Sprite, Texture>(spriteID))
				{
					sf::Sprite& sprite = sprites[spriteID].sprite;
					sprite.setPosition(Gametrans[spriteID].pos.x, Gametrans[spriteID].pos.y);
					sprite.setRotation(Gametrans[spriteID].rot);
					sprite.setTexture(*textures[spriteID].texture);
					GameScene.draw(sprite);
				}
			}
		}
	}

	for(unsigned ID = 0; ID<_uiEnv->maxEntities(); ID++)
	{
		auto GUIobjs = _uiEnv->get<GUIObj>();

		// blur the contents behind the menu
		if(sf::Shader::isAvailable())
		{
			if(_uiEnv->getEntityName(ID)=="ESC UI") /// \NOTE: If there is a memory error within this scope, check getEntityName for bugs
			{
				auto visible = _uiEnv->get<StdComponent<bool>>();  /// \TODO: keep teack of this when breating other stdcomponent bools
				auto blur = _uiEnv->get<StdComponent<sf::Shader>>();

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
		if(_uiEnv->hasComponents<Transform, UserInterface, Label, StdComponent<sf::RectangleShape>>(ID))
		{
			auto ui = _uiEnv->get<UserInterface>();

			if(ui[ID].show)
			{
				if(GUIobjs[ID].type==GUIObj::BUTTON)
				{
					auto labels = _uiEnv->get<Label>();
					auto button = _uiEnv->get<StdComponent<sf::RectangleShape>>();

					const float margin = 50;
					sf::FloatRect dim = sf::FloatRect(uiTrans[ID].pos.x, uiTrans[ID].pos.y,
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
						uiTrans[ID].pos = sf::Mouse::getPosition();
					}

					UIScene.draw(labels[ID].label);
				}
				else if(GUIobjs[ID].type==GUIObj::PANE)
				{
					auto labels = _uiEnv->get<Label>();
					auto button = _uiEnv->get<StdComponent<sf::RectangleShape>>();

					const float margin = 50;
					sf::FloatRect dim = sf::FloatRect(uiTrans[ID].pos.x, uiTrans[ID].pos.y,
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
		if(_mainEnv->hasComponents<Transform, Label>(ID))
		{
			if(_mainEnv->getEntityName(ID)=="CPU")
			{
				if(*_mainEnv->get<StdComponent<bool>>()[ID].data)
				{
					auto labels = _mainEnv->get<Label>();
					UIScene.draw(labels[ID].label);
				}
			}
			
		}
	}

	_win->draw(sf::Sprite(GameScene.getTexture()), shader);						     GameScene.clear(sf::Color(0, 0, 0, 0)); GameScene.display();
	_win->draw(sf::Sprite(UIScene.getTexture()), sf::RenderStates(sf::BlendAlpha));  UIScene.clear(sf::Color(0, 0, 0, 0));   UIScene.display();
	_win->display();
}
