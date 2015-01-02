#include "managers/CPU_Manager.h"
#include "../include/Components.h"

CPUManager::CPUManager() {}


CPUManager::~CPUManager() {}

void CPUManager::createCPUMgr(Environment* _env, sf::RenderWindow* _win)
{
	unsigned id = _env->createEntity
	(
		"CPU",
		new Transform(Vec2f(100, 100)),
		new Label(""),
		Component(bool, "visible", new bool(true))
	);

	if(_env->hasComponents<Label>(id))
	{
		auto labels = _env->get<Label>();
			if(!labels[id].font.loadFromFile("res/arial.ttf")) { cout<<"ERROR: FONT NOT FOUND"<<endl; exit(2); }
			labels[id].label.setFont(labels[id].font);
			labels[id].label.setCharacterSize(12);
			labels[id].label.setColor(sf::Color(sf::Color::Black));
			labels[id].label.setStyle(sf::Text::Style::Bold);
			labels[id].label.setPosition(sf::Vector2f(_win->getSize().x-400, 10));
		label = _env->cpuData = &labels[id].label;
	}
}

sf::Time CPUManager::update()
{
	sf::Time elapsed = clock.restart();
	std::string renderSys = label->getString();

	int i = renderSys.size()-1;
	if(i>0)
	{
		for(; renderSys[i-1]!='\n'; --i);
		renderSys = renderSys.substr(i, renderSys.size());
	}

	label->setString("Elapsed time: "+std::to_string(elapsed.asMicroseconds()/1000.0)+" ms\t"+std::to_string(1.0/elapsed.asSeconds())+" FPS\n"+renderSys);
	return elapsed;
}
