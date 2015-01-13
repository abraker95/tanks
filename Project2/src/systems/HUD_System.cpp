#include "systems/HUD_System.h"
#include "components.h"
#include "utils.h"
#include "events.h"

HUDSystem::HUDSystem(Environment* _env)
{
	bar = sf::RectangleShape();
	clock = sf::Clock();

	for(unsigned i = 0; i<numTanks; i++)
	{
		IDs[i] = _env->getID("tank"+std::to_string(i+1)); /// \NOTE: The tanks must follow the naming convention
		_env->getComponent<Label>(IDs[i]).label.setString("Player "+std::to_string(i+1)); /// \TODO: Make so that players can name Tanks

		auto label = _env->getComponent<Label>(IDs[i]);
			if(!label.font.loadFromFile("res/arial.ttf")) cout<<"ERROR: FONT NOT FOUND"<<endl;
			label.label.setFont(label.font);
			label.label.setCharacterSize(9);
			label.label.setString("Player "+std::to_string(i+1)); /// \TODO: Make so that players can name Tanks

		cout<<"Set label ID: "<<IDs[i]<<endl;
	}
}


HUDSystem::~HUDSystem()
{}

void HUDSystem::update(Environment* _env, sf::RenderTexture& _HUDScene)
{
	// if the game got restarted, get the new tank ID's
	if(_env->getEvents<NewGameEvent>().size()>0)
	{
		cout<<"test"<<endl;
		for(unsigned i = 0; i<numTanks; i++)
		{
			IDs[i] = _env->getID("tank"+std::to_string(i+1)); /// \NOTE: The tanks must follow the naming convention

			auto label = _env->getComponent<Label>(IDs[i]);
				if(!label.font.loadFromFile("res/arial.ttf")) cout<<"ERROR: FONT NOT FOUND"<<endl;
				label.label.setFont(label.font);
				label.label.setCharacterSize(9);
				label.label.setString("Player "+std::to_string(i+1)); /// \TODO: Make so that players can name Tanks			
		}
	}


	for(unsigned i = 0; i<numTanks; i++)
	{
		unsigned ID = IDs[i];
		auto trans = _env->getComponent<Transform>(ID);
		auto sprites = _env->getComponent<Sprite>(ID);
		auto health = _env->getComponent<Health>(ID);
		auto gun = _env->getComponent<Gun>(ID);
		auto labels = _env->getComponent<Label>(ID);
		auto tankControls = _env->getComponent<TankControls>(ID);

		if(ID!=0 && health.getHealth()!= 0) // if it exists and not dead
		{
			sf::Color barColor;
			const float barThickness = 5,
					    maxBarHeight = 80;
			float barHeight;
			bar.setRotation(0);

			// Healthbar
			barHeight = health.getHealth()*(maxBarHeight/health.getMaxHealth()); // barHeight = Min(val*(maxHeight/maxVal), maxHeight)
			bar.setPosition(trans.pos.x+sprites.sprite.getLocalBounds().width/2+20, trans.pos.y+sprites.sprite.getLocalBounds().height/2+5-barHeight);
			bar.setSize(sf::Vector2f(barThickness, barHeight));
				 if(BTWN(maxBarHeight*0/4, bar.getSize().y, maxBarHeight*1/4)) barColor = sf::Color(sf::Color(255, 0,   0, 255));  // red 
			else if(BTWN(maxBarHeight*1/4, bar.getSize().y, maxBarHeight*2/4)) barColor = sf::Color(sf::Color(255, 102, 0, 255));  // orange
			else if(BTWN(maxBarHeight*2/4, bar.getSize().y, maxBarHeight*3/4)) barColor = sf::Color(sf::Color(255, 255, 0, 255));  // yellow
			else if(BTWN(maxBarHeight*3/4, bar.getSize().y, maxBarHeight*4/4)) barColor = sf::Color(sf::Color(0,   255, 0, 255));  // green
			bar.setFillColor(barColor);			
			
			_HUDScene.draw(bar);

			// Cooldown bar
		    barHeight = MIN(gun.fireClock.getElapsedTime().asMilliseconds()*(maxBarHeight/(gun.fireCooldown*1000)), maxBarHeight);
			bar.setPosition(trans.pos.x+sprites.sprite.getLocalBounds().width/2+30, trans.pos.y+sprites.sprite.getLocalBounds().height/2+5-barHeight);
			bar.setSize(sf::Vector2f(barThickness, barHeight)); 
			bar.setFillColor(sf::Color(101, 153, 255, 255)); // light-blueish

			_HUDScene.draw(bar);

			// Tank stats
			const int showTime = 5000;
			int ms = clock.getElapsedTime().asMilliseconds();
			float fadeTime = 255.0/showTime;

			for(int j = 0; j<5; j++)
			{
				if(sf::Keyboard::isKeyPressed(tankControls.keys[j]))
					if(tankControls.state.test(j))
						clock.restart();
			}

			/// \TODO: Make a function to convert view cordinates to screen coordinates
			bool cursorOnTank = BTWN(sprites.sprite.getGlobalBounds().left, sf::Mouse::getPosition().x, sprites.sprite.getGlobalBounds().width)
							 && BTWN(sprites.sprite.getGlobalBounds().top,  sf::Mouse::getPosition().y, sprites.sprite.getGlobalBounds().height);
			bool show = (ms > showTime) || cursorOnTank;
			if(show)
			{
				bar.setFillColor(sf::Color(51, 103, 205, MIN((ms-5000.0)*5*fadeTime, 255))); // dark-blueish
				bar.setPosition(trans.pos.x+20, trans.pos.y-sprites.sprite.getLocalBounds().height);
				bar.setSize(sf::Vector2f(2, 20));
				bar.setRotation(45);
				_HUDScene.draw(bar);

				bar.setPosition(trans.pos.x+80, trans.pos.y-sprites.sprite.getLocalBounds().height);
				bar.setSize(sf::Vector2f(2, 60));
				bar.setRotation(90);
				_HUDScene.draw(bar);

				labels.label.setPosition(sf::Vector2f(trans.pos.x+25, trans.pos.y-sprites.sprite.getLocalBounds().height-labels.label.getCharacterSize()-3));
				labels.label.setColor(sf::Color(51, 103, 205, MIN((ms-5000.0)*5*fadeTime, 255)));

				cout<<"Update label ID: "<<ID<<endl;
				_HUDScene.draw(labels.label);  /// \TODO (abraker): figure out what is making this crash
			}
		}
	}
}