#include "managers/UI_Manager.h"
#include "Components.h"
#include "events.h"

UI_Manager::UI_Manager()
{}


UI_Manager::~UI_Manager()
{}

void UI_Manager::CreateButton(Environment* _env, Vec2f _pos, std::function<void*()> _action, std::string _lable, std::string _name)
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

	/// \NOTE: I have ABSOLUTELY no idea why &GUIobjs[button].action works while creating this component with &action in the createEntity funtion creates a wierd pointer error.
	_env->addComponents(button, new UserInterface(std::bitset<UIstates>(1<<UserInterface::HIGHLIGHT|1<<UserInterface::CLICK|1<<UserInterface::PRESS),
						&GUIobjs[button].action));

	/// \TODO: This is DANGEROUS if label is not a component of this entity
	/// \TODO: Figure out why the font files is not being found
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
}

void UI_Manager::CreateMenu(Environment* _env, sf::RenderWindow* _win)
{
	std::function<void*()> quitAction = [this]()->void*
	{ 
	 exit(0);  
	 return nullptr; 
	};
	CreateButton(_env, Vec2f(200.f, 100.f), quitAction, "Quit", "Quit_Button");

	std::function<void*()> ToggleFullscreen = [_env, _win]()->void*
	{
		auto WindowMode = _env->getEvents<WindowModeEvent>();

		if(WindowMode.size() > 0)
		{
			if(*WindowMode[0].fullscreen==false)
				_win->create(sf::VideoMode::getDesktopMode(), "https://github.com/Sherushe/tanks.git (pre-alpha branch)", sf::Style::Fullscreen);
			else
				_win->create(sf::VideoMode(1024, 720), "https://github.com/Sherushe/tanks.git (pre-alpha branch)", sf::Style::Resize);

			*WindowMode[0].fullscreen = !*WindowMode[0].fullscreen;
			//_env->emit(new WindowModeEvent(WindowMode[0].fullscreen));
		}
		return nullptr;
	};
	CreateButton(_env, Vec2f(200.f, 220.f), ToggleFullscreen, "Fullscreen / Windowed", "Full_Win_Button");


	auto NewGame = [_env]()->void*
	{
		_env->emit(new NewGameEvent());
		return nullptr;
	};
	CreateButton(_env, Vec2f(800.f, 220.f), NewGame, "New Game", "New_Game_Button");


	_env->createEntity("ESC UI",
		new GUIObj(GUIObj::VOID, [this]()->void* { return nullptr; }),
		Component(bool, "visible", new bool(true)),
		Component(sf::Texture, "shader_filter", nullptr),
		Component(sf::Shader, "blur_shader", nullptr)
		);
}