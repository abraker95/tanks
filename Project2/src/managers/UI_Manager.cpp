#include "managers/UI_Manager.h"
#include "Components.h"
#include "events.h"
#include "managers/managers.h"

UI_Manager::UI_Manager()
{}


UI_Manager::~UI_Manager()
{}

unsigned UI_Manager::CreateButton(Environment* _env, Vec2f _pos, std::function<void*()> _action, std::string _label, std::string _name, MENU _subMenu)
{
	unsigned int button = _env->createEntity
	(
		_name,
		new Transform(_pos),
		new GUIObj(GUIObj::BUTTON, _action),
		new Label(_label),
		new StdComponent<sf::RectangleShape>(new sf::RectangleShape()),
		new StdComponent<MENU>(new MENU(_subMenu)),
		new UserInterface(std::bitset<UIstates>(1<<UserInterface::HIGHLIGHT|1<<UserInterface::CLICK|1<<UserInterface::PRESS), _subMenu, _action)
	);

	/// \TODO: make use of the font manager
	auto labels = _env->get<Label>();
		if(!labels[button].font.loadFromFile("res/arial.ttf")) cout<<"ERROR: FONT NOT FOUND"<<endl;
		labels[button].label.setFont(labels[button].font);

		const float margin = 50;
		sf::FloatRect textSize = labels[button].label.getLocalBounds();

		int charSize = 24;
		labels[button].label.setCharacterSize(charSize);
		labels[button].label.setPosition(sf::Vector2f(_pos.x+margin/2, _pos.y+margin/2-charSize/2));


	auto bounds = _env->get<StdComponent<sf::RectangleShape>>();
		 bounds[button].data->setPosition(_pos.x, _pos.y);
		 bounds[button].data->setSize(sf::Vector2f(textSize.width+margin, textSize.height+margin));

	return button;
}

unsigned UI_Manager::CreatePane(Environment* _env, Vec2f _pos, std::string _label, std::string _name, MENU _subMenu)
{
	unsigned int pane = _env->createEntity
	(
		_name,
		new Transform(_pos),
		new GUIObj(GUIObj::PANE, nullptr),
		new Label(_label),
		new StdComponent<sf::RectangleShape>(new sf::RectangleShape()),
		new UserInterface(std::bitset<UIstates>(NULL), _subMenu)
	);

	/// \TODO: make use of the font manager
	auto labels = _env->get<Label>();
		if(!labels[pane].font.loadFromFile("res/arial.ttf")) cout<<"ERROR: FONT NOT FOUND"<<endl;
		labels[pane].label.setFont(labels[pane].font);

		const float margin = 50;
		sf::Text text = labels[pane].label;
		sf::FloatRect textSize = text.getLocalBounds();

		int charSize = 24;
		labels[pane].label.setCharacterSize(charSize);
		labels[pane].label.setPosition(sf::Vector2f(_pos.x+margin/2, _pos.y+margin/2-charSize/2));
		labels[pane].label.setColor(sf::Color::Black);


	auto bounds = _env->get<StdComponent<sf::RectangleShape>>();
	     bounds[pane].data->setPosition(_pos.x, _pos.y);
		 bounds[pane].data->setSize(sf::Vector2f(textSize.width+margin, textSize.height+margin));

	return pane;
}

unsigned UI_Manager::CreateTextField(Environment* _env, Vec2f _pos, std::string _name, MENU _subMenu, unsigned _maxChar)
{
	unsigned int textField = _env->createEntity
	(
		_name,
		new Transform(_pos),
		new GUIObj(GUIObj::TEXTFIELD, nullptr),
		new Label(""),
		Component(sf::RectangleShape, "", new sf::RectangleShape()),
		Component(unsigned, "", new unsigned(_maxChar)),
		new UserInterface(std::bitset<UIstates>(1<<UserInterface::KEY |1<<UserInterface::CLICK | 1<<UserInterface::FOCUS), _subMenu)
	);

	/// \TODO: make use of the font manager
	auto labels = _env->get<Label>();
		if(!labels[textField].font.loadFromFile("res/arial.ttf")) cout<<"ERROR: FONT NOT FOUND"<<endl;
		labels[textField].label.setFont(labels[textField].font);

		sf::Text text = labels[textField].label;
		sf::FloatRect textSize = text.getLocalBounds();

		int charSize = 24;
		labels[textField].label.setCharacterSize(charSize);
		labels[textField].label.setPosition(sf::Vector2f(_pos.x, _pos.y));
		labels[textField].label.setColor(sf::Color::Black);

	return textField;
}

void UI_Manager::CreateMenu(Environment* _env, sf::RenderWindow* _win, Managers* _mgrs, bool& fullscreen)
{
	CreateTitleScreen(_env, _mgrs);
	CreateMainSubMenu(_env, _mgrs);
	CreateOptionsSubMenu(_env, _win, fullscreen);
	CreateAboutSubMenu(_env);
	CreateGameOverSubMenu(_env, _mgrs);
	CreateVoidSubMenu(_env, _mgrs);
	CreateChangeNameSubMenu(_env);
	CreateNetSubMenu(_env, _mgrs);
	CreateNewGameSubMenu(_env, _mgrs);

	currMenu = TITLE_SCREEN;
}

void UI_Manager::ShowGameOver()
{
	currMenu = UI_Manager::GAME_OVER;
}


bool UI_Manager::isVisible(UserInterface* _UI)
{
	return (_UI->subMenu==currMenu);
}

void UI_Manager::CreateTitleScreen(Environment* _env, Managers* _mgrs)
{
	auto NewGame = [_env, this]()->void*
	{
		currMenu = NEWGAME_MENU;
		return nullptr;
	};
	CreateButton(_env, Vec2f(100.f, 340.f), NewGame, "New Game", "New_Game_Button", UI_Manager::TITLE_SCREEN);

	std::function<void*()> About = [_env, this]()->void*
	{
		currMenu = ABOUT_MENU;
		return nullptr;
	};
	CreateButton(_env, Vec2f(650.f, 340.f), About, "About", "About_Button", UI_Manager::TITLE_SCREEN);

	std::function<void*()> quitAction = [this]()->void*
	{
		exit(0);
		return nullptr;
	};
	CreateButton(_env, Vec2f(400.f, 520.f), quitAction, "Quit", "Quit_Button", UI_Manager::TITLE_SCREEN);
}

void UI_Manager::CreateMainSubMenu(Environment* _env, Managers* _mgrs)
{
	auto NewGame = [_env, _mgrs, this]()->void*
	{
		_mgrs->game_manager.ResetGame(_env, _mgrs);
		currMenu = NO_MENU;
		return nullptr;
	};
	CreateButton(_env, Vec2f(100.f, 220.f), NewGame, "New Game", "New_Game_Button", UI_Manager::MAIN_MENU);

	auto Options = [_env, this]()->void*
	{
		/*
		/// \TODO:
		Implement Player tank colors, Volume, and key Controls buttons
		*/
		currMenu = OPTIONS_MENU;
		return nullptr;
	};
	CreateButton(_env, Vec2f(650.f, 220.f), Options, "Options", "Options_Button", UI_Manager::MAIN_MENU);

	std::function<void*()> quitAction = [_env, _mgrs, this]()->void*
	{
		if(_mgrs->game_manager.isOnline())
			_mgrs->net_manager.closeConnection();
		_mgrs->game_manager.EndGame(_env, _mgrs);
		currMenu = TITLE_SCREEN;
		return nullptr;
	};
	CreateButton(_env, Vec2f(400.f, 520.f), quitAction, "Quit to Title", "Quit_Button", UI_Manager::MAIN_MENU);
}

void UI_Manager::CreateNewGameSubMenu(Environment* _env, Managers* _mgrs)
{
	auto NewGame = [_env, _mgrs, this]()->void*
	{
		_mgrs->game_manager.NewLocalGame(_env, _mgrs);
		currMenu = NO_MENU;
		return nullptr;
	};
	CreateButton(_env, Vec2f(100.f, 220.f), NewGame, "New Local Game", "New_Game_Button", UI_Manager::NEWGAME_MENU);

	std::function<void*()> netMenu = [_env, this]()->void*
	{
		currMenu = NET_MENU;
		return nullptr;
	};
	CreateButton(_env, Vec2f(650.f, 220.f), netMenu, "Online", "Net_Button", UI_Manager::NEWGAME_MENU);

	std::function<void*()> back = [_env, this]()->void*
	{
		currMenu = TITLE_SCREEN;
		return nullptr;
	};
	CreateButton(_env, Vec2f(400.f, 520.f), back, "back", "Back_Button", UI_Manager::NEWGAME_MENU);
}

void UI_Manager::CreateOptionsSubMenu(Environment* _env, sf::RenderWindow* _win, bool& fullscreen)
{
	std::function<void*()> ToggleFullscreen = [_env, _win, &fullscreen]()->void*
	{
		if(!fullscreen)
			_win->create(sf::VideoMode::getDesktopMode(), "https://github.com/Sherushe/tanks.git (pre-alpha branch)", sf::Style::Fullscreen);
		else
			_win->create(sf::VideoMode(1024, 720), "https://github.com/Sherushe/tanks.git (pre-alpha branch)", sf::Style::Resize);
		
		fullscreen = !fullscreen;

		// Note(Sherushe): when a new window is created no sf::Event::Resize is sent
		// so a ResizeEvent is sent from here instead of the input system
		_env->emit(new ResizeEvent(_win->getSize().x, _win->getSize().y));

		//_win->setFramerateLimit(60);
		// *WindowMode[0].fullscreen = !*WindowMode[0].fullscreen;
		//_env->emit(new WindowModeEvent(WindowMode[0].fullscreen));
		return nullptr;
	};

	CreateButton(_env, Vec2f(600.f, 220.f), ToggleFullscreen, "Fullscreen / Windowed", "fullWin_Button", UI_Manager::OPTIONS_MENU);

	std::function<void*()> changeName = [_env, this]()->void*
	{
		currMenu = CHANGENAME_MENU;
		return nullptr;
	};
	CreateButton(_env, Vec2f(200.f, 220.f), changeName, "Player Names", "PlayerNames_Button", UI_Manager::OPTIONS_MENU);

	std::function<void*()> optionsBack = [_env, this]()->void*
	{
		currMenu = MAIN_MENU;
		return nullptr;
	};
	CreateButton(_env, Vec2f(600.f, 420.f), optionsBack, "Back", "optionsBack_Button", UI_Manager::OPTIONS_MENU);
}

void UI_Manager::CreateChangeNameSubMenu(Environment* _env)
{	
	CreatePane(_env, Vec2f(100.f, 200.f), "Player 1", "IP Text", UI_Manager::CHANGENAME_MENU);
	CreatePane(_env, Vec2f(100.f, 280.f), "Player 2", "IP Text", UI_Manager::CHANGENAME_MENU);

	CreateTextField(_env, Vec2f(250.f, 213.f), "Player1Name_TextInput", UI_Manager::CHANGENAME_MENU);
	CreateTextField(_env, Vec2f(250.f, 293.f), "Player2Name_TextInput", UI_Manager::CHANGENAME_MENU);

	std::function<void*()> Done = [_env, this]()->void*
	{
		unsigned tank1ID = _env->getID("tank1"),
				 tank2ID = _env->getID("tank2"),
				 txtFld1ID = _env->getID("Player1Name_TextInput"),
				 txtFld2ID = _env->getID("Player2Name_TextInput");

		if(tank1ID != -1) _env->get<Label>()[tank1ID].label.setString(_env->get<Label>()[txtFld1ID].label.getString());
		if(tank2ID != -1) _env->get<Label>()[tank2ID].label.setString(_env->get<Label>()[txtFld2ID].label.getString());

		currMenu = OPTIONS_MENU;
		return nullptr;
	};
	CreateButton(_env, Vec2f(400.f, 420.f), Done, "Done", "Done_Button", UI_Manager::CHANGENAME_MENU);
}

void UI_Manager::CreateAboutSubMenu(Environment* _env)
{
	std::function<void*()> aboutBack = [_env, this]()->void*
	{
		currMenu = TITLE_SCREEN;
		return nullptr;
	};
	CreateButton(_env, Vec2f(450.f, 620.f), aboutBack, "Back", "aboutBack_Button", UI_Manager::ABOUT_MENU);

	std::string aboutInfo =
		" Tank Game \n"\
		" Created by: ABraker and Sherushe";
	CreatePane(_env, Vec2f(300.f, 200.f), aboutInfo, "About Text", ABOUT_MENU);
}

void UI_Manager::CreateNetSubMenu(Environment* _env, Managers* _mgrs)
{
	std::string IPinfo = "IP Address:                                ";
	CreatePane(_env, Vec2f(200.f, 200.f), IPinfo, "IP Text", NET_MENU);
	CreateTextField(_env, Vec2f(360.f, 213.f), "IP_TextInput", UI_Manager::NET_MENU, 15);

	std::function<void*()> connect = [_env, _mgrs, this]()->void*
	{
		/// \TODO: Connect to IP Address code
		std::string IPAddress = _env->get<Label>()[_env->getID("IP_TextInput")].label.getString();
		cout<<"IP address to connect to: "<<IPAddress<<endl;
		
		_mgrs->net_manager.InitOnlineMode(NetManager::CLIENT);
		_mgrs->game_manager.NewNetGame(_env, _mgrs);
		_mgrs->net_manager.setHostIP(IPAddress);
		_mgrs->net_manager.connectToHost(IPAddress);
		

		/*if(playerNum != 0) // if host hasnt left before client joins
		{
		//	_mgrs->game_manager.NewNetGame(_env, _mgrs);
			for(int i = 1; i<=playerNum; i++)
				_mgrs->game_manager.playerJoin(_env, _mgrs, i, (i==playerNum)); // Create players and map the playerNums

			currMenu = UI_Manager::NO_MENU;
		}*/

		currMenu = UI_Manager::NO_MENU;
		return nullptr;
	};
	CreateButton(_env, Vec2f(200.f, 320.f), connect, "Connect", "Connect_Button", UI_Manager::NET_MENU);

	std::function<void*()> host = [_env, _mgrs, this]()->void*
	{
		/// \TODO: If hosting a net game, close it first
		std::string IPAddress = _mgrs->net_manager.getHostIP();
		cout<<"Host IP address: "<<IPAddress<<endl;
		if(_mgrs->net_manager.InitOnlineMode(NetManager::HOST))
		{
			_mgrs->game_manager.NewNetGame(_env, _mgrs);
			_mgrs->game_manager.playerJoin(_env, _mgrs, 1, true); // Create host player
			currMenu = UI_Manager::NO_MENU;
		}	
		else
			cout<<"Error making a net game."<<endl;
		
		return nullptr;
	};
	CreateButton(_env, Vec2f(650.f, 320.f), host, "Create Server", "Host_Button", UI_Manager::NET_MENU);

	std::function<void*()> netBack = [_env, this]()->void*
	{
		currMenu = NEWGAME_MENU;
		return nullptr;
	};
	CreateButton(_env, Vec2f(450.f, 520.f), netBack, "Back", "NetBack_Button", UI_Manager::NET_MENU);
}

void UI_Manager::CreateGameOverSubMenu(Environment* _env, Managers* _mgrs)
{
	std::string GameOverMsg = " Game Over! ";
	CreatePane(_env, Vec2f(300.f, 200.f), GameOverMsg, "Game Over Text", UI_Manager::GAME_OVER);

	auto PlayAgain = [_env, _mgrs, this]()->void*
	{
	//	_env->emit(new NewGameEvent(false));
		_mgrs->game_manager.ResetGame(_env, _mgrs);
		currMenu = UI_Manager::NO_MENU;
		return nullptr;
	};
	CreateButton(_env, Vec2f(200.f, 420.f), PlayAgain, "Play Again", "Play_Again_Button", UI_Manager::GAME_OVER);

	auto EndGame = [_env, _mgrs, this]()->void*
	{
		_mgrs->game_manager.EndGame(_env, _mgrs);
		currMenu = UI_Manager::TITLE_SCREEN;
		return nullptr;
	};
	CreateButton(_env, Vec2f(400.f, 420.f), EndGame, "End Game", "End_Game_Button", UI_Manager::GAME_OVER);
}

void UI_Manager::CreateVoidSubMenu(Environment* _env, Managers* _mgrs)
{
	std::function<void*()> ESC = [_env, _mgrs, this]()->void*
	{
		auto UI = _env->get<UserInterface>();

			 if(currMenu==OPTIONS_MENU)		currMenu = MAIN_MENU;
		else if(currMenu==ABOUT_MENU)		currMenu = MAIN_MENU;
		else if(currMenu==MAIN_MENU)		currMenu = NO_MENU;
		else if(currMenu==NO_MENU)			currMenu = MAIN_MENU;
		else if(currMenu==CHANGENAME_MENU)  currMenu = OPTIONS_MENU;

		if(currMenu == NO_MENU)	    _mgrs->game_manager.ResumeGame();
		else						_mgrs->game_manager.PauseGame();

		return nullptr;
	};
	int esc_UI = _env->createEntity("ESC UI",
		new GUIObj(GUIObj::VOID, ESC),
		Component(bool, "visible", new bool(true)),
		Component(sf::Shader, "blur_shader", new sf::Shader())
		);
	_env->addComponents(esc_UI, new UserInterface(std::bitset<UIstates>(), NO_MENU, ESC));

	sf::Shader* shader = _env->get<StdComponent<sf::Shader>>()[esc_UI].data;
	if(!shader->loadFromFile("res/blur.vert", "res/blur.frag"))
		std::cout<<"ERROR: Could not load shader"<<std::endl;
	shader->setParameter("intensity", 0.004f);

	visible = _env->get<StdComponent<bool>>()[esc_UI].data;
}
