#include "systems/render_system.h"
#include "systems/HUD_System.h"
#include "Components.h"
#include "utils.h"
#include "events.h"

RenderSystem::RenderSystem(sf::RenderWindow* _win, FontManager* font_manager)
{
	GameScene.create(_win->getSize().x, _win->getSize().y);
	UIScene.create(_win->getSize().x, _win->getSize().y);
	shader = nullptr;

	debug_infos.setFont(font_manager->load("res/Arial.ttf"));
	debug_infos.setCharacterSize(14);
	debug_infos.setColor(sf::Color(sf::Color::Blue));
	debug_infos.setStyle(sf::Text::Style::Bold);
	debug_infos.setPosition(sf::Vector2f(_win->getSize().x - 400, 10));
}

RenderSystem::~RenderSystem()
{}

void RenderSystem::update(Environment* _env, HUDSystem* _HUDSystem, Managers* managers, sf::RenderWindow* _win, const std::string& monitoring_results)
{
	auto sprites = _env->get<Sprite>();
	auto trans = _env->get<Transform>();
	auto textures = _env->get<Texture>();
	auto vertex_array = _env->get<VertexArray>();
	auto view_controller = _env->get<ViewController>();

	auto resize_event = _env->getEvents<ResizeEvent>();

	if(resize_event.size() > 0)
	{
		GameScene.create(resize_event[0].width, resize_event[0].height);
		UIScene.create(resize_event[0].width, resize_event[0].height);
	}

	/*
		Note(Sherushe): All events should be polled in the main loop (application.cpp)
	    Note(abraker): Ok... :(
	sf::Event event;
	while(_win->pollEvent(event))
	{
		if(event.type==sf::Event::Resized)
		{
			cout<<"resize"<<endl;
			/// Help with UI resizing here
		}
	}	
	*/
	if(managers->game_manager.getGameState()!=GameManager::GAMESTATE::ENDED)
	{
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
		_HUDSystem->update(_env, managers, _win, GameScene);
	}

	// if the game is on the Title Screen, draw title backround
	if(managers->game_manager.getGameState()==GameManager::GAMESTATE::ENDED)
	{
		// let there be a plain backround for now				
		GameScene.clear(sf::Color(173, 214, 255, 255));
	}


	for(unsigned ID = 0; ID<_env->maxEntities(); ID++)
	{
		auto GUIobjs = _env->get<GUIObj>();
		auto visible = _env->get<StdComponent<bool>>();  /// \TODO: keep track of this when creating other stdcomponent bools

		if(managers->game_manager.getGameState()==GameManager::GAMESTATE::PAUSED)
		{
			// blur the contents behind the menu
			if(sf::Shader::isAvailable())
			{
				if(_env->getEntityName(ID)=="ESC UI")
				{
					auto blur = _env->get<StdComponent<sf::Shader>>();

					if(GUIobjs[ID].type==GUIObj::VOID)
					{
						if(*visible[ID].data && visible[ID].name=="visible")
						{
							blur[ID].data->setParameter("tex0", GameScene.getTexture());
							shader = blur[ID].data;
						}
					}
				}
			}
		}
		else if(managers->game_manager.getGameState()==GameManager::GAMESTATE::PLAYING)
			shader = nullptr;

		if(managers->game_manager.getGameState()!=GameManager::GAMESTATE::PLAYING)
		{
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

						sf::IntRect dim = sf::IntRect(UIScene.mapCoordsToPixel(sf::Vector2f(trans[ID].pos.x, trans[ID].pos.y)),
							UIScene.mapCoordsToPixel(sf::Vector2f(MAX(labels[ID].label.getLocalBounds().width, 200), labels[ID].font.getLineSpacing(labels[ID].label.getCharacterSize()))));
						Vec2i pos = sf::Mouse::getPosition(*_win);
						if(BTWN(dim.left, pos.x, dim.left+dim.width)&&BTWN(dim.top, pos.y, dim.top+dim.height))
							ui[ID].cursorOnThis = true;
						else
							ui[ID].cursorOnThis = false;

						UIScene.draw(labels[ID].label);

						sf::RectangleShape textbox;
						textbox.setPosition(sf::Vector2f(dim.left, dim.top));
						textbox.setSize(sf::Vector2f(dim.width, dim.height));
						textbox.setFillColor(sf::Color(173, 214, 255, 50));
						UIScene.draw(textbox);

						if(ui[ID].state.test(UserInterface::FOCUS))
						{
							sf::RectangleShape textCursor;
							textCursor.setPosition(sf::Vector2f(dim.left+labels[ID].label.getLocalBounds().width+3, dim.top));
							textCursor.setSize(sf::Vector2f(2, dim.height));
							textCursor.setFillColor(sf::Color(0, 0, 0, 255));
							UIScene.draw(textCursor);
						}

					}
				}
			}
		}
	}

	debug_infos.setString(monitoring_results);
	UIScene.draw(debug_infos);

	_win->draw(sf::Sprite(GameScene.getTexture()), shader);						     GameScene.clear(sf::Color(0, 0, 0, 0)); GameScene.display();
	_win->draw(sf::Sprite(UIScene.getTexture()), sf::RenderStates(sf::BlendAlpha));  UIScene.clear(sf::Color(0, 0, 0, 0));   UIScene.display();
	_win->display();
}
