#include "managers/UI_Manager.h"
#include "Components.h"
#include "events.h"

UI_Manager::UI_Manager()
{}


UI_Manager::~UI_Manager()
{}

unsigned UI_Manager::CreateButton(Environment* _env, Vec2f _pos, std::function<void*()> _action, std::string _lable, std::string _name, MENU _subMenu)
{
	unsigned int button = _env->createEntity
	(
		_name,
		new Transform(_pos),
		new GUIObj(GUIObj::BUTTON, _action),
		new Label(_lable),
		new StdComponent<sf::RectangleShape>(new sf::RectangleShape()),
		new StdComponent<MENU>(new MENU(_subMenu)),
		new UserInterface(std::bitset<UIstates>(1<<UserInterface::HIGHLIGHT|1<<UserInterface::CLICK|1<<UserInterface::PRESS), _subMenu, _action)
	);

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

unsigned UI_Manager::CreatePane(Environment* _env, Vec2f _pos, std::string _lable, std::string _name, MENU _subMenu)
{
	unsigned int pane = _env->createEntity
	(
		_name,
		new Transform(_pos),
		new GUIObj(GUIObj::PANE, nullptr),
		new Label(_lable),
		new StdComponent<sf::RectangleShape>(new sf::RectangleShape()),
		new UserInterface(std::bitset<UIstates>(NULL), _subMenu)
	);

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

unsigned UI_Manager::CreateTextField(Environment* _env, Vec2f _pos, std::string _name, MENU _subMenu)
{
	unsigned int textField = _env->createEntity
	(
		_name,
		new Transform(_pos),
		new GUIObj(GUIObj::TEXTFIELD, nullptr),
		new Label(""),
		Component(sf::RectangleShape, "", new sf::RectangleShape()),
		new UserInterface(std::bitset<UIstates>(1<<UserInterface::KEY |1<<UserInterface::CLICK | 1<<UserInterface::FOCUS), _subMenu)
	);

	auto labels = _env->get<Label>();
		if(!labels[textField].font.loadFromFile("res/arial.ttf")) cout<<"ERROR: FONT NOT FOUND"<<endl;
		labels[textField].label.setFont(labels[textField].font);

		sf::Text text = labels[textField].label;
		sf::FloatRect textSize = text.getLocalBounds();

		int charSize = 24;
		labels[textField].label.setCharacterSize(charSize);
		labels[textField].label.setPosition(sf::Vector2f(_pos.x, _pos.y));
		labels[textField].label.setColor(sf::Color::Black);
		labels[textField].label.setString("Player");

	return textField;
}

void UI_Manager::CreateMenu(Environment* _env, sf::RenderWindow* _win)
{
	CreateMainSubMenu(_env);
	CreateOptionsSubMenu(_env, _win);
	CreateAboutSubMenu(_env);
	CreateGameOverSubMenu(_env);
	CreateVoidSubMenu(_env);
	CreateChangeNameSubMenu(_env);

	currMenu = MAIN_MENU;
}

void UI_Manager::ShowGameOver()
{
	currMenu = UI_Manager::GAME_OVER;
}


bool UI_Manager::isVisible(UserInterface* _UI)
{
	return (_UI->subMenu==currMenu);
}


void UI_Manager::CreateMainSubMenu(Environment* _env)
{
	std::function<void*()> quitAction = [this]()->void*
	{
		exit(0);
		return nullptr;
	};
	CreateButton(_env, Vec2f(600.f, 340.f), quitAction, "Quit", "Quit_Button", UI_Manager::MAIN_MENU);

	auto NewGame = [_env, this]()->void*
	{
		_env->emit(new NewGameEvent(true));
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
	CreateButton(_env, Vec2f(600.f, 220.f), Options, "Options", "Options_Button", UI_Manager::MAIN_MENU);

	std::function<void*()> About = [_env, this]()->void*
	{
		currMenu = ABOUT_MENU;
		return nullptr;
	};
	CreateButton(_env, Vec2f(100.f, 340.f), About, "About", "About_Button", UI_Manager::MAIN_MENU);
}

void UI_Manager::CreateOptionsSubMenu(Environment* _env, sf::RenderWindow* _win)
{
	std::function<void*()> ToggleFullscreen = [_env, _win]()->void*
	{
		auto WindowMode = _env->getEvents<WindowModeEvent>();

		if(WindowMode.size() > 0)
		{
			if(*WindowMode[0].fullscreen==false)
				_win->create(sf::VideoMode::getDesktopMode(), "https://github.com/Sherushe/tanks.git (pre-alpha branch)", sf::Style::Fullscreen);
			else
				_win->create(sf::VideoMode(1024, 720), "https://github.com/Sherushe/tanks.git (pre-alpha branch)", sf::Style::Resize);

			//_win->setFramerateLimit(60);
			*WindowMode[0].fullscreen = !*WindowMode[0].fullscreen;
			//_env->emit(new WindowModeEvent(WindowMode[0].fullscreen));
		}
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
	CreateButton(_env, Vec2f(200.f, 420.f), optionsBack, "Back", "optionsBack_Button", UI_Manager::OPTIONS_MENU);
}

void UI_Manager::CreateChangeNameSubMenu(Environment* _env)
{	
	CreateTextField(_env, Vec2f(200.f, 220.f), "Player1Name_TextInput", UI_Manager::CHANGENAME_MENU);
	CreateTextField(_env, Vec2f(600.f, 220.f), "Player2Name_TextInput", UI_Manager::CHANGENAME_MENU);

	std::function<void*()> Done = [_env, this]()->void*
	{
		unsigned tank1ID = _env->getID("tank1"),
				 tank2ID = _env->getID("tank2"),
				 txtFld1ID = _env->getID("Player1Name_TextInput"),
				 txtFld2ID = _env->getID("Player2Name_TextInput");

		_env->get<Label>()[tank1ID].label.setString(_env->get<Label>()[txtFld1ID].label.getString());
		_env->get<Label>()[tank2ID].label.setString(_env->get<Label>()[txtFld2ID].label.getString());

		currMenu = OPTIONS_MENU;
		return nullptr;
	};
	CreateButton(_env, Vec2f(400.f, 420.f), Done, "Done", "Done_Button", UI_Manager::CHANGENAME_MENU);
}

void UI_Manager::CreateAboutSubMenu(Environment* _env)
{
	std::function<void*()> aboutBack = [_env, this]()->void*
	{
		currMenu = MAIN_MENU;
		return nullptr;
	};
	CreateButton(_env, Vec2f(450.f, 620.f), aboutBack, "Back", "aboutBack_Button", UI_Manager::ABOUT_MENU);

	std::string aboutInfo =
		" Tank Game \n"\
		" Created by: ABraker and Sherushe";
	CreatePane(_env, Vec2f(300.f, 200.f), aboutInfo, "About Text", ABOUT_MENU);
}

void UI_Manager::CreateGameOverSubMenu(Environment* _env)
{
	std::string GameOverMsg = " Game Over! ";
	CreatePane(_env, Vec2f(300.f, 200.f), GameOverMsg, "Game Over Text", UI_Manager::GAME_OVER);

	auto PlayAgain = [_env, this]()->void*
	{
		_env->emit(new NewGameEvent(false));
		currMenu = UI_Manager::NO_MENU;
		return nullptr;
	};
	CreateButton(_env, Vec2f(200.f, 420.f), PlayAgain, "Play Again", "Play_Again_Button", UI_Manager::GAME_OVER);

	auto EndGame = [_env, this]()->void*
	{
		currMenu = UI_Manager::MAIN_MENU;
		return nullptr;
	};
	CreateButton(_env, Vec2f(400.f, 420.f), EndGame, "End Game", "End_Game_Button", UI_Manager::GAME_OVER);
}

void UI_Manager::CreateVoidSubMenu(Environment* _env)
{
	std::function<void*()> ESC = [_env, this]()->void*
	{
		auto UI = _env->get<UserInterface>();

			 if(currMenu==OPTIONS_MENU)		currMenu = MAIN_MENU;
		else if(currMenu==ABOUT_MENU)		currMenu = MAIN_MENU;
		else if(currMenu==MAIN_MENU)		currMenu = NO_MENU;
		else if(currMenu==NO_MENU)			currMenu = MAIN_MENU;
		else if(currMenu==CHANGENAME_MENU)  currMenu = OPTIONS_MENU;

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