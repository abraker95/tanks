#pragma once
#include <cmath>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "Bullet.h"
#include "Environment.h"

/** \NOTE: Tank Game Object
	Spawn Case:
		env: Environment Creation.
		\TODO: gameplay cases
	Destroy Case:
		env: Environment destruction
		\TODO: gameplay cases
*/
class Tank : public GameObject
{
public:
	Tank();
	virtual ~Tank();

	// just moves the tank, missiles spawing etc is managed at a higher level
	void Update(float _elapsedTime);

	int getNumMissiles() const { return numMissiles; }
	int getNumMines() const { return numMines; }
	int getNumWalls() const { return numWalls; }

	void addMissiles(int _numMissiles) { numMissiles += _numMissiles; }
	void addMines(int _numMines) { numMines += _numMines; }
	void addWalls(int _numWalls) { numWalls += _numWalls; }

private:
	int numMissiles;
	int numMines;
	int numWalls;

	enum KeyMap
	{
		Turn_Right = 0,
		Turn_Left,
		Go_Forward,
		Go_Backward,
		Fire_Bullet
	};

	const float fireCooldown = 0.1f;
	Clock fireClock;

	void UpdateUserInput();
	bool Fire();
};
