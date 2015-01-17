#include "systems/render_system.h"
#include "systems/HUD_System.h"
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

void RenderSystem::update(Environment* _env, HUDSystem* _HUDSystem, sf::RenderWindow* _win, CPUManager* _cpuMgr)
{
	auto sprites = _env->get<Sprite>();
	auto trans = _env->get<Transform>();
	auto textures = _env->get<Texture>();
	auto vertex_array = _env->get<VertexArray>();
	auto view_controller = _env->get<ViewController>();

	auto WindowMode = _env->getEvents<WindowModeEvent>();
	if(WindowMode.size()>0)
		fullscreen = *WindowMode[0].fullscreen;
	
	if(prevFullscreen != fullscreen)
	{
		GameScene.create(_win->getSize().x, _win->getSize().y);
		UIScene.create(_win->getSize().x, _win->getSize().y);    
	}
	prevFullscreen = fullscreen;
	_env->emit(new WindowModeEvent(&fullscreen));

	sf::Event event;
	while(_win->pollEvent(event))
	{
		if(event.type==sf::Event::Resized)
		{
			cout<<"resize"<<endl;
			/// Help with UI resizing here
		}
	}	

	for(unsigned viewID = 0; viewID<_env->maxEntities(); viewID++)
	{
		if(_env->hasComponents<ViewController>(viewID))
		{
			GameScene.setView(view_controller[viewID].view);
			for(unsigned mapID = 0; mapID<_env->maxEntities(); mapID++)
			{
				if(_env->hasComponents<VertexArray, Texture, Tilemap>(mapID))
				{
					GameScene.draw(*vertex_array[mapID].array, textures[mapID].texture);
					break; // should only be one map
				}
			}

			// temporary fix
			for(unsigned spriteID = 0; spriteID<_env->maxEntities(); spriteID++)
			{
				if(_env->hasComponents<Transform, Sprite, Texture>(spriteID))
				{
					sf::Sprite& sprite = sprites[spriteID].sprite;
					sprite.setPosition(trans[spriteID].pos.x, trans[spriteID].pos.y);
					sprite.setRotation(trans[spriteID].rot);
					sprite.setTexture(*textures[spriteID].texture);
					GameScene.draw(sprite);
				}
			}
			//_env->updateWrapper(_HUDSystem, GameScene);
		}
	}
	_HUDSystem->update(_env, _win, GameScene);

	for(unsigned ID = 0; ID<_env->maxEntities(); ID++)
	{
		auto GUIobjs = _env->get<GUIObj>();
		auto visible = _env->get<StdComponent<bool>>();  /// \TODO: keep track of this when creating other stdcomponent bools
		// blur the contents behind the menu
		if(sf::Shader::isAvailable())
		{
			if(_env->getEntityName(ID)=="ESC UI") /// \NOTE: If there is a memory error within this scope, check getEntityName for bugs
			{
				auto blur = _env->get<StdComponent<sf::Shader>>();

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
		if(_env->hasComponents<Transform, UserInterface, Label, StdComponent<sf::RectangleShape>>(ID))
		{
			auto ui = _env->get<UserInterface>();

			if(ui[ID].show)
			{
				if(GUIobjs[ID].type==GUIObj::BUTTON)
				{
					auto labels = _env->get<Label>();
					auto button = _env->get<StdComponent<sf::RectangleShape>>();

					const float margin = 50;
					sf::IntRect dim = sf::IntRect(UIScene.mapCoordsToPixel(sf::Vector2f(trans[ID].pos.x, trans[ID].pos.y)),
												  UIScene.mapCoordsToPixel(sf::Vector2f(labels[ID].label.getLocalBounds().width+margin, labels[ID].label.getLocalBounds().height+margin)));
								
					button[ID].data->setSize(sf::Vector2f(dim.width, dim.height));
					button[ID].data->setFillColor(sf::Color(50, 50, 100, 255));
					UIScene.draw(*button[ID].data);

					Vec2i pos = sf::Mouse::getPosition(*_win);
					//dim.left += 1024-_win->getSize().x;
					//dim.top += 720-_win->getSize().y;
					//cout<<"dim.left: "<<dim.left<<endl;
					
					//	PRINT_DEBUG(std::cout<<" X pos view: "<<trans[i].x<<" X pos screen: "<<pos.x<<std::endl, HI_DEBUG, GFXSYS);
					//	PRINT_DEBUG(std::cout<<" X range: "<<trans[i].x<<" "<<pos.x-window->getPosition().x<<" "<<bounds.width+trans[i].x<<std::endl, HI_DEBUG, GFXSYS);
					//	PRINT_DEBUG(std::cout<<" Y range: "<<trans[i].y<<" "<<sf::Mouse::getPosition().y-window->getPosition().y<<" "<<bounds.height+trans[i].y<<std::endl<<endl, HI_DEBUG, GFXSYS);

					/// \TODO: Make this apply to all GUI objects by putting this into a more general scope. This requires GUI entities to have a dim component
					if(BTWN(dim.left, pos.x, dim.left+dim.width)&&BTWN(dim.top, pos.y, dim.top+dim.height))
						ui[ID].cursorOnThis = true;
					else
						ui[ID].cursorOnThis = false;
					
					if(ui[ID].state.test(UserInterface::PRESS))
					{
						button[ID].data->setFillColor(sf::Color(50, 50, 50, 50)); /// \NOTE: Fill color is overlayed ontop of the existing fill
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
					auto labels = _env->get<Label>();
					auto button = _env->get<StdComponent<sf::RectangleShape>>();

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
				else if(GUIobjs[ID].type==GUIObj::TEXTFIELD)
				{
					auto labels = _env->get<Label>();
					auto trans = _env->get<Transform>();
						labels[ID].label.setPosition(trans[ID].pos.x, trans[ID].pos.y);
						labels[ID].label.setColor(sf::Color(sf::Color::Black));
						labels[ID].label.setCharacterSize(24);

					sf::IntRect dim = sf::IntRect(UIScene.mapCoordsToPixel(sf::Vector2f(trans[ID].pos.x, trans[ID].pos.y)),
												  UIScene.mapCoordsToPixel(sf::Vector2f(labels[ID].label.getLocalBounds().width, labels[ID].label.getLocalBounds().height)));
					Vec2i pos = sf::Mouse::getPosition(*_win);
					if(BTWN(dim.left, pos.x, dim.left+dim.width)&&BTWN(dim.top, pos.y, dim.top+dim.height))
						ui[ID].cursorOnThis = true;
					else
						ui[ID].cursorOnThis = false;

					if(ui[ID].state.test(UserInterface::FOCUS))
					{
						cout<<"focus"<<endl;
						sf::Event event;
						sf:string str = labels[ID].label.getString();

						while(_win->pollEvent(event))
						{
							if(event.type == sf::Event::TextEntered)
							{
								// Handle ASCII characters only
								if(event.text.unicode < 128)
								{
									str += static_cast<char>(event.text.unicode);
									//labels[ID].label.SetText(str);
								}
							}
						}
						
					}
					UIScene.draw(labels[ID].label);
				}
			}
		}
	}


	for(unsigned ID = 0; ID<_env->maxEntities(); ID++)
	{
		if(_env->hasComponents<Transform, Label>(ID))
		{
			if(_env->getEntityName(ID)=="CPU")
			{
				if(*_env->get<StdComponent<bool>>()[ID].data)
				{
					auto labels = _env->get<Label>();
					UIScene.draw(labels[ID].label);
				}
			}
			
		}
	}

	_win->draw(sf::Sprite(GameScene.getTexture()), shader);						     GameScene.clear(sf::Color(0, 0, 0, 0)); GameScene.display();
	_win->draw(sf::Sprite(UIScene.getTexture()), sf::RenderStates(sf::BlendAlpha));  UIScene.clear(sf::Color(0, 0, 0, 0));   UIScene.display();
	_win->display();
}
