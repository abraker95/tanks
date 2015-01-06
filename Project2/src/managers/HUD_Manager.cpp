#include "managers/HUD_Manager.h"
#include "../include/Components.h"

HUD_Manager::HUD_Manager()
{}


HUD_Manager::~HUD_Manager()
{}

/* TODO:
	
*/

void HUD_Manager::createAmmoHUD(Environment* _env)
{
	unsigned id = _env->createEntity
	(
		"AmmoHUD",
		new Sprite(),
		new Transform(Vec2f(100, 100)),
		Component(bool, "visible", new bool(true))
	);
}

void HUD_Manager::createHealthHUD(Environment* _env)
{

}

void HUD_Manager::createLivesHUD(Environment* _env)
{

}

void HUD_Manager::createHUD(Environment* _env)
{

}


