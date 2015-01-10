#include "managers/UI_Manager.h"
#include "Components.h"
#include "events.h"

UI_Manager::UI_Manager()
{}


UI_Manager::~UI_Manager()
{}

int UI_Manager::CreateButton(Environment* _uiEnv, Vec2f _pos, std::function<void*()> _action, std::string _lable, std::string _name, bool _visible)
{
	unsigned int button = _uiEnv->createEntity
	(
		_name,
		new Transform(_pos),
		new GUIObj(GUIObj::BUTTON, _action),
		new Label(_lable),
		new StdComponent<sf::RectangleShape>(new sf::RectangleShape()),
		new UserInterface(std::bitset<UIstates>(1<<UserInterface::HIGHLIGHT|1<<UserInterface::CLICK|1<<UserInterface::PRESS), _action)
	);

	auto GUIobjs = _uiEnv->get<GUIObj>();
	auto labels = _uiEnv->get<Label>();
	auto UI = _uiEnv->get<UserInterface>();

	UI[button].show = _visible;

	if(!labels[button].font.loadFromFile("res/arial.ttf")) cout<<"ERROR: FONT NOT FOUND"<<endl;
	labels[button].label.setFont(labels[button].font);

	const float margin = 50;
	sf::Text text = labels[button].label;
	sf::FloatRect textSize = text.getLocalBounds();

	int charSize = 24;
	labels[button].label.setCharacterSize(charSize);
	labels[button].label.setPosition(sf::Vector2f(_pos.x+margin/2, _pos.y+margin/2-charSize/2));


	auto bounds = _uiEnv->get<StdComponent<sf::RectangleShape>>();
		 bounds[button].data->setPosition(_pos.x, _pos.y);
		 bounds[button].data->setSize(sf::Vector2f(textSize.width+margin, textSize.height+margin));

	IDs.push_back(button);
	return button;
}

int UI_Manager::CreatePane(Environment* _uiEnv, Vec2f _pos, std::string _lable, std::string _name, bool _visible)
{
	unsigned int pane = _uiEnv->createEntity
	(
		_name,
		new Transform(_pos),
		new GUIObj(GUIObj::PANE, nullptr),
		new Label(_lable),
		new StdComponent<sf::RectangleShape>(new sf::RectangleShape()),
		new UserInterface(std::bitset<UIstates>(NULL), nullptr)
	);

	auto GUIobjs = _uiEnv->get<GUIObj>();
	auto labels = _uiEnv->get<Label>();
	auto UI = _uiEnv->get<UserInterface>();

	UI[pane].show = _visible;

	if(!labels[pane].font.loadFromFile("res/arial.ttf")) cout<<"ERROR: FONT NOT FOUND"<<endl;
	labels[pane].label.setFont(labels[pane].font);

	const float margin = 50;
	sf::Text text = labels[pane].label;
	sf::FloatRect textSize = text.getLocalBounds();

	int charSize = 24;
	labels[pane].label.setCharacterSize(charSize);
	labels[pane].label.setPosition(sf::Vector2f(_pos.x+margin/2, _pos.y+margin/2-charSize/2));
	labels[pane].label.setColor(sf::Color::Black);


	auto bounds = _uiEnv->get<StdComponent<sf::RectangleShape>>();
	bounds[pane].data->setPosition(_pos.x, _pos.y);
	bounds[pane].data->setSize(sf::Vector2f(textSize.width+margin, textSize.height+margin));

	IDs.push_back(pane);
	return pane;
}

void UI_Manager::CreateMenu(Environment* _mainEnv, Environment* _uiEnv, sf::RenderWindow* _win)
{
	std::function<void*()> quitAction = [this]()->void*
	{ 
	 exit(0);  
	 return nullptr; 
	};
	mainMenu.push_back(CreateButton(_uiEnv, Vec2f(600.f, 340.f), quitAction, "Quit", "Quit_Button"));

	auto NewGame = [_mainEnv]()->void*
	{
		_mainEnv->emit(new NewGameEvent());
		return nullptr;
	};
	mainMenu.push_back(CreateButton(_uiEnv, Vec2f(100.f, 220.f), NewGame, "New Game", "New_Game_Button"));

	auto Options = [_uiEnv, this]()->void*
	{
		/*
		  /// \TODO:  
			Implement Player tank colors, Volume, and key Controls buttons
		*/
		auto UI = _uiEnv->get<UserInterface>();
		for(unsigned int i = 0; i<mainMenu.size(); i++)	   UI[mainMenu[i]].show = !UI[mainMenu[i]].show;
		for(unsigned int i = 0; i<optionsMenu.size(); i++) UI[optionsMenu[i]].show = !UI[optionsMenu[i]].show;

		currMenu = OPTIONS_MENU;		
		return nullptr;
	};
	mainMenu.push_back(CreateButton(_uiEnv, Vec2f(600.f, 220.f), Options, "Options", "Options_Button"));

	std::function<void*()> About = [_uiEnv, this]()->void*
	{
		auto UI = _uiEnv->get<UserInterface>();
		for(unsigned int i = 0; i<mainMenu.size(); i++)	   UI[mainMenu[i]].show = !UI[mainMenu[i]].show;
		for(unsigned int i = 0; i<aboutMenu.size(); i++)   UI[aboutMenu[i]].show = !UI[aboutMenu[i]].show;

		currMenu = ABOUT_MENU;
		return nullptr;
	};
	mainMenu.push_back(CreateButton(_uiEnv, Vec2f(100.f, 340.f), About, "About", "About_Button"));

	std::function<void*()> ToggleFullscreen = [_mainEnv, _win]()->void*
	{
		auto WindowMode = _mainEnv->getEvents<WindowModeEvent>();

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
	optionsMenu.push_back(CreateButton(_uiEnv, Vec2f(600.f, 220.f), ToggleFullscreen, "Fullscreen / Windowed", "Full_Win_Button", false));

	std::function<void*()> optionsBack = [_uiEnv, this]()->void*
	{
		auto UI = _uiEnv->get<UserInterface>();
		for(unsigned int i = 0; i<mainMenu.size(); i++)	   UI[mainMenu[i]].show = !UI[mainMenu[i]].show;
		for(unsigned int i = 0; i<optionsMenu.size(); i++) UI[optionsMenu[i]].show = !UI[optionsMenu[i]].show;

		currMenu = MAIN_MENU;
		return nullptr;
	};
	optionsMenu.push_back(CreateButton(_uiEnv, Vec2f(200.f, 220.f), optionsBack, "Back", "optionsBack_Button", false));

	std::function<void*()> aboutBack = [_uiEnv, this]()->void*
	{
		auto UI = _uiEnv->get<UserInterface>();
		for(unsigned int i = 0; i<mainMenu.size(); i++)	   UI[mainMenu[i]].show = !UI[mainMenu[i]].show;
		for(unsigned int i = 0; i<aboutMenu.size(); i++)   UI[aboutMenu[i]].show = !UI[aboutMenu[i]].show;

		currMenu = MAIN_MENU;
		return nullptr;
	};
	aboutMenu.push_back(CreateButton(_uiEnv, Vec2f(450.f, 620.f), aboutBack, "Back", "aboutBack_Button", false));

	std::string aboutInfo =
		" Tank Game \n"\
		" Created by: ABraker and Sherushe";
	aboutMenu.push_back(CreatePane(_uiEnv, Vec2f(300.f, 200.f), aboutInfo, "About Text", false));

	std::function<void*()> ESC = [_uiEnv, this, aboutBack]()->void*
	{
		auto UI = _uiEnv->get<UserInterface>();
		
		if(currMenu == OPTIONS_MENU)
			for(unsigned int i = 0; i<optionsMenu.size(); i++) UI[optionsMenu[i]].show = !UI[optionsMenu[i]].show;
		else if(currMenu==MAIN_MENU)
			for(unsigned int i = 0; i<mainMenu.size(); i++)    UI[mainMenu[i]].show = !UI[mainMenu[i]].show;
		else if(currMenu==ABOUT_MENU)
			aboutBack();
		
		*visible = UI[optionsMenu[0]].show || UI[mainMenu[0]].show;
		return nullptr;
	};
	int esc_UI = _uiEnv->createEntity("ESC UI",
		new GUIObj(GUIObj::VOID, ESC),
		Component(bool, "visible", new bool(true)),
		Component(sf::Shader, "blur_shader", new sf::Shader())
		);
		_uiEnv->addComponents(esc_UI, new UserInterface(std::bitset<UIstates>(), ESC));

		sf::Shader* shader = _uiEnv->get<StdComponent<sf::Shader>>()[esc_UI].data;
			if(!shader->loadFromFile("res/blur.vert", "res/blur.frag"))
				std::cout<<"ERROR: Could not load shader"<<std::endl;
			shader->setParameter("intensity", 0.004f);
	
	currMenu = MAIN_MENU;
	visible = _uiEnv->get<StdComponent<bool>>()[esc_UI].data;
	IDs.push_back(esc_UI);
}