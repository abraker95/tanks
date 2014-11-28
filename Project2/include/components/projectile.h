#pragma once
#include "core/environment.h"

struct Projectile
{
	Projectile();
	Projectile(int damage, unsigned shooting_entity);
	
	int damage;
	unsigned shooting_entity;
};
