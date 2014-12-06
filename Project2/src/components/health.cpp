#include "components/health.h"

Health::Health(int health, int max_health) : health(health), max_health(max_health) 
{
}
	
void Health::addHealth(int _health) { health += _health; }
bool Health::hasHealth() const { return health > 0; }
int  Health::getHealth() const { return health; }
void Health::resetHealth() { health = max_health; }
int Health::getMaxHealth() const { return max_health; }
