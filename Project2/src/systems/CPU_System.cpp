#include "systems/CPU_System.h"
#include "../include/Components.h"

CPUSystem::CPUSystem(Environment* _env, sf::RenderWindow* _win)
{
	/// \TODO: Show this be in CPU Manager?
	int id = _env->getID("CPU");
	if(_env->hasComponents<Label>(id))
	{
		auto labels = _env->get<Label>();
			if(!labels[id].font.loadFromFile("res/arial.ttf")) { cout<<"ERROR: FONT NOT FOUND"<<endl; exit(2); }
			labels[id].label.setFont(labels[id].font);
			labels[id].label.setCharacterSize(12);
			labels[id].label.setColor(sf::Color(sf::Color::Black));
			labels[id].label.setStyle(sf::Text::Style::Bold);
			labels[id].label.setPosition(sf::Vector2f(_win->getSize().x-400, 10));
		label = &labels[id].label;
	}
}


CPUSystem::~CPUSystem()
{}

void CPUSystem::update(Environment* _env)
{
	label->setString("");
}
