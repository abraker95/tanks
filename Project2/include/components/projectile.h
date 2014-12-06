#pragma once
#include "core/environment.h"

struct Projectile : public Component<Projectile>
{
	Projectile(int damage, unsigned shooting_entity);
	
	int damage;
	unsigned shooting_entity;
};
