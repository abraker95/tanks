#pragma once
#include <SFML/Graphics.hpp>
#include "Object.h"

class Tank : public Object
{
public:
	Tank(const char* filename);
	virtual ~Tank();

	// just moves the tank, missiles spawing etc is managed at a higher level
	void Update(float elapsedTime);

	// getters and setters
	// putting the defintion so that the compiler can inline them
	void 	setHealth(int _health) { health = _health; }
	int 	getHealth() const { return health; }
	void 	setNumMines(int _numMines) { numMines = _numMines; }
	int 	getNumMines() const	{ return numMines; }
	void 	setNumMissiles(int _numMissiles) { numMissiles = _numMissiles; }
	int 	getNumMissiles() const { return numMissiles; }
	void 	setNumWalls(int _numWalls) { numWalls = _numWalls; }
	int 	getNumWalls() const { return numWalls; }

	void 	setTankSpeed(float _tankSpeed) { tankSpeed = _tankSpeed; }
	float 	getTankSpeed() const { return tankSpeed; }
	void 	setTankAngleSpeed(float _tankAngleSpeed) { tankAngleSpeed = _tankAngleSpeed; }
	float 	getTankAngleSpeed() const { return tankAngleSpeed; }

private:
	int health;
	int numMissiles;
	int numMines;
	int numWalls;

	float tankSpeed;
	float tankAngleSpeed;
};
