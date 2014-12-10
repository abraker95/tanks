#pragma once

#include "core/environment.h"

struct Health : public Component<Health>
{
	int health;
	int max_health;

	Health(int health, int max_health);
	
	void addHealth(int _health);
	bool hasHealth() const;
	int  getHealth() const;
	void resetHealth();
	int getMaxHealth() const;

};
