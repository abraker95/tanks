#pragma once

struct Health
{
	int health;
	int max_health;

	Health();
	Health(int health, int max_health);
	
	void addHealth(int _health);
	bool hasHealth() const;
	int  getHealth() const;
	void resetHealth();
	int getMaxHealth() const;

};
