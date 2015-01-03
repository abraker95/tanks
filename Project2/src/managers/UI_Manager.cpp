#include "managers/UI_Manager.h"
#include "Components.h"
#include "events.h"

UI_Manager::UI_Manager()
{}


UI_Manager::~UI_Manager()
{}

int UI_Manager::CreateButton(Environment* _env, Vec2f _pos, std::function<void*()> _action, std::string _lable, std::string _name, bool _visible)
{
	unsigned int button = _env->createEntity
	(
		_name,
		new Transform(_pos),
		new GUIObj(GUIObj::BUTTON, _action),
		new Label(_lable),
		new StdComponent<sf::RectangleShape>(new sf::RectangleShape())
	);

	auto GUIobjs = _env->get<GUIObj>();
	auto labels = _env->get<Label>();

	/// \NOTE (abraker): I have ABSOLUTELY no idea why &GUIobjs[button].action works while creating this component with &action in the createEntity funtion creates a wierd pointer error.
	_env->addComponents(button, new UserInterface(std::bitset<UIstates>(1<<UserInterface::HIGHLIGHT|1<<UserInterface::CLICK|1<<UserInterface::PRESS),
						&GUIobjs[button].action));

	auto UI = _env->get<UserInterface>();
	UI[button].show = _visible;

	if(!labels[button].font.loadFromFile("res/arial.ttf")) cout<<"ERROR: FONT NOT FOUND"<<endl;
	labels[button].label.setFont(labels[button].font);

	const float margin = 50;
	sf::Text text = labels[button].label;
	sf::FloatRect textSize = text.getLocalBounds();

	int charSize = 24;
	labels[button].label.setCharacterSize(charSize);
	labels[button].label.setPosition(sf::Vector2f(_pos.x+margin/2, _pos.y+margin/2-charSize/2));


	auto bounds = _env->get<StdComponent<sf::RectangleShape>>();
		 bounds[button].data->setPosition(_pos.x, _pos.y);
		 bounds[button].data->setSize(sf::Vector2f(textSize.width+margin, textSize.height+margin));

	return button;
}

void UI_Manager::CreateMenu(Environment* _env, sf::RenderWindow* _win)
{
	std::function<void*()> quitAction = [this]()->void*
	{ 
	 exit(0);  
	 return nullptr; 
	};
	mainMenu.push_back(CreateButton(_env, Vec2f(600.f, 340.f), quitAction, "Quit", "Quit_Button"));

	auto NewGame = [_env]()->void*
	{
		_env->emit(new NewGameEvent());
		return nullptr;
	};
	mainMenu.push_back(CreateButton(_env, Vec2f(100.f, 220.f), NewGame, "New Game", "New_Game_Button"));

	auto Options = [_env, this]()->void*
	{
		/*
		  /// \TODO:  
			Implement Player tank colors, Volume, and key Controls buttons
		*/
		auto UI = _env->get<UserInterface>();
		for(unsigned int i = 0; i<mainMenu.size(); i++)	   UI[mainMenu[i]].show = !UI[mainMenu[i]].show;
		for(unsigned int i = 0; i<optionsMenu.size(); i++) UI[optionsMenu[i]].show = !UI[optionsMenu[i]].show;

		currMenu = OPTIONS_MENU;		
		return nullptr;
	};
	mainMenu.push_back(CreateButton(_env, Vec2f(600.f, 220.f), Options, "Options", "Options_Button"));

	std::function<void*()> About = [_env, this]()->void*
	{
		auto UI = _env->get<UserInterface>();
		for(unsigned int i = 0; i<mainMenu.size(); i++)	   UI[mainMenu[i]].show = !UI[mainMenu[i]].show;
		for(unsigned int i = 0; i<aboutMenu.size(); i++)   UI[aboutMenu[i]].show = !UI[aboutMenu[i]].show;

		currMenu = ABOUT_MENU;
		return nullptr;
	};
	mainMenu.push_back(CreateButton(_env, Vec2f(100.f, 340.f), About, "About", "About_Button"));

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
	optionsMenu.push_back(CreateButton(_env, Vec2f(600.f, 220.f), ToggleFullscreen, "Fullscreen / Windowed", "Full_Win_Button", false));

	std::function<void*()> optionsBack = [_env, this]()->void*
	{
		auto UI = _env->get<UserInterface>();
		for(unsigned int i = 0; i<mainMenu.size(); i++)	   UI[mainMenu[i]].show = !UI[mainMenu[i]].show;
		for(unsigned int i = 0; i<optionsMenu.size(); i++) UI[optionsMenu[i]].show = !UI[optionsMenu[i]].show;

		currMenu = MAIN_MENU;
		return nullptr;
	};
	optionsMenu.push_back(CreateButton(_env, Vec2f(200.f, 220.f), optionsBack, "Back", "optionsBack_Button", false));

	std::function<void*()> aboutBack = [_env, this]()->void*
	{
		auto UI = _env->get<UserInterface>();
		for(unsigned int i = 0; i<mainMenu.size(); i++)	   UI[mainMenu[i]].show = !UI[mainMenu[i]].show;
		for(unsigned int i = 0; i<aboutMenu.size(); i++)   UI[aboutMenu[i]].show = !UI[aboutMenu[i]].show;

		currMenu = MAIN_MENU;
		return nullptr;
	};
	aboutMenu.push_back(CreateButton(_env, Vec2f(450.f, 620.f), aboutBack, "Back", "aboutBack_Button", false));

	std::function<void*()> ESC = [_env, this, &aboutBack]()->void*
	{
		auto UI = _env->get<UserInterface>();
		
		if(currMenu == OPTIONS_MENU)
			for(unsigned int i = 0; i<optionsMenu.size(); i++) UI[optionsMenu[i]].show = !UI[optionsMenu[i]].show;
		else if(currMenu==MAIN_MENU)
			for(unsigned int i = 0; i<mainMenu.size(); i++)    UI[mainMenu[i]].show = !UI[mainMenu[i]].show;
		else if(currMenu==ABOUT_MENU)
			aboutBack();
		
		*visible = UI[optionsMenu[0]].show || UI[mainMenu[0]].show;
		return nullptr;
	};
	int esc_UI = _env->createEntity("ESC UI",
		new GUIObj(GUIObj::VOID, ESC),
		Component(bool, "visible", new bool(true)),
		Component(sf::Texture, "shader_filter", nullptr),
		Component(sf::Shader, "blur_shader", nullptr)
		);

	currMenu = MAIN_MENU;
	visible = _env->get<StdComponent<bool>>()[esc_UI].data;
}