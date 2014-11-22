#pragma once

struct Health
{
	int health;
	int max_health;

	Health(int health, int max_health) : health(health), max_health(max_health) {}
	
	void addHealth(int _health) { health += _health; }
	bool hasHealth() const { return health > 0; }
	int  getHealth() const { return health; }
	void resetHealth() { health = max_health; }
	int getMaxHealth() const { return max_health; }

};
