#include "systems/HUD_System.h"
#include "components.h"
#include "utils.h"
#include "events.h"

HUDSystem::HUDSystem(Environment* _env)
{
	bar = sf::RectangleShape();
	clock = sf::Clock();
}


HUDSystem::~HUDSystem()
{}

void HUDSystem::update(Environment* _env, Managers* managers, sf::Window* _win, sf::RenderTexture& _HUDScene)
{
	if(managers->game_manager.getGameState() == GameManager::PLAYING)
	{
		for(unsigned i = 1; i<=managers->game_manager.getNumPlayers(); i++)
		{
			unsigned ID = managers->game_manager.getPlayer(i);
			auto trans = _env->get<Transform>();
			auto sprites = _env->get<Sprite>();
			auto health = _env->get<Health>();
			auto gun = _env->get<Gun>();
			auto labels = _env->get<Label>();
			auto tankControls = _env->get<TankControls>();
			auto player = _env->get<Player>();

			if(ID!=0 && health[ID].getHealth()!=0) // if it exists and not dead
			{
				sf::Color barColor;
				const float barThickness = 5,
					maxBarHeight = 80;
				float barHeight;
				bar.setRotation(0);

				// Healthbar
				barHeight = health[ID].getHealth()*(maxBarHeight/health[ID].getMaxHealth()); // barHeight = Min(val*(maxHeight/maxVal), maxHeight)
				bar.setPosition(trans[ID].pos.x+sprites[ID].sprite.getLocalBounds().width/2+20, trans[ID].pos.y+sprites[ID].sprite.getLocalBounds().height/2+5-barHeight);
				bar.setSize(sf::Vector2f(barThickness, barHeight));
				if(BTWN(maxBarHeight*0/4, bar.getSize().y, maxBarHeight*1/4)) barColor = sf::Color(sf::Color(255, 0, 0, 255));  // red 
				else if(BTWN(maxBarHeight*1/4, bar.getSize().y, maxBarHeight*2/4)) barColor = sf::Color(sf::Color(255, 102, 0, 255));  // orange
				else if(BTWN(maxBarHeight*2/4, bar.getSize().y, maxBarHeight*3/4)) barColor = sf::Color(sf::Color(255, 255, 0, 255));  // yellow
				else if(BTWN(maxBarHeight*3/4, bar.getSize().y, maxBarHeight*4/4)) barColor = sf::Color(sf::Color(0, 255, 0, 255));  // green
				bar.setFillColor(barColor);

				_HUDScene.draw(bar);

				// Cooldown bar
				barHeight = MIN(gun[ID].fireClock.getElapsedTime().asMilliseconds()*(maxBarHeight/(gun[ID].fireCooldown*1000)), maxBarHeight);
				bar.setPosition(trans[ID].pos.x+sprites[ID].sprite.getLocalBounds().width/2+30, trans[ID].pos.y+sprites[ID].sprite.getLocalBounds().height/2+5-barHeight);
				bar.setSize(sf::Vector2f(barThickness, barHeight));
				bar.setFillColor(sf::Color(101, 153, 255, 255)); // light-blueish

				_HUDScene.draw(bar);


				// Tank stats
				const int showTime = 5000;
				int ms = clock.getElapsedTime().asMilliseconds();
				float fadeTime = 255.0/showTime;

				sf::Vector2i tankPos = _HUDScene.mapCoordsToPixel(sf::Vector2f(trans[ID].pos.x-sprites[ID].sprite.getGlobalBounds().width/2, trans[ID].pos.y-sprites[ID].sprite.getGlobalBounds().height/2)),
					tankSizePos = _HUDScene.mapCoordsToPixel(sf::Vector2f(trans[ID].pos.x+sprites[ID].sprite.getGlobalBounds().width/2, trans[ID].pos.y+sprites[ID].sprite.getGlobalBounds().height/2));
				sf::View prevView = _HUDScene.getView();
				_HUDScene.setView(_HUDScene.getDefaultView()); // reset view for the stats

				for(int j = 0; j<5; j++)
				{
					if(sf::Keyboard::isKeyPressed(tankControls[ID].keys[j]))
						if(tankControls[ID].state.test(j))
							clock.restart();
				}

				/// \TODO: Make a function to convert view cordinates to screen coordinates
				bool cursorOnTank = BTWN(tankPos.x, sf::Mouse::getPosition().x-_win->getPosition().x, tankSizePos.x)
					&&BTWN(tankPos.y, sf::Mouse::getPosition().y-_win->getPosition().y, tankSizePos.y);
				bool show = (ms > showTime)||cursorOnTank;
				if(show)
				{
					float shiftCorrX = prevView.getSize().x*40/_win->getSize().x,
						shiftCorrY = prevView.getSize().y*20/_win->getSize().y;

					bar.setFillColor(sf::Color(51, 103, 205, MIN((ms-5000.0)*5*fadeTime, 255))); // dark-blueish
					bar.setPosition(tankPos.x-shiftCorrX+110, tankPos.y+shiftCorrY-70);
					bar.setSize(sf::Vector2f(2, 40));
					bar.setRotation(45);
					_HUDScene.draw(bar);

					bar.setPosition(tankPos.x-shiftCorrX+260, tankPos.y+shiftCorrY-70);
					bar.setSize(sf::Vector2f(2, 150));
					bar.setRotation(90);
					_HUDScene.draw(bar);

					labels[ID].label.setPosition(tankPos.x-shiftCorrX+130, tankPos.y+shiftCorrY-100);
					labels[ID].label.setColor(sf::Color(51, 103, 205, MIN((ms-5000.0)*5*fadeTime, 255)));
					/// \ TODO: Apply name changes when changed in the settings
					_HUDScene.draw(labels[ID].label);

					sf::Text score;
					score.setFont(labels[ID].font);
					score.setColor(labels[ID].label.getColor());
					score.setCharacterSize(labels[ID].label.getCharacterSize());
					int win, loss;
					managers->score_manager.getStatus(player[ID].player_id, win, loss);
					score.setString("W: "+to_string(win)+"   L: "+to_string(loss));
					score.setPosition(tankPos.x-shiftCorrX+130, tankPos.y+shiftCorrY-60);
					_HUDScene.draw(score);
				}
				_HUDScene.setView(prevView);
			}
		}
	}
}
