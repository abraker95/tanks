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

	/*struct Inputmap
	{
		Inputmap() {};
		Inputmap(const Keyboard::Key _turnRight, 
				 const Keyboard::Key _turnLeft, 
				 const Keyboard::Key _goForward, 
				 const Keyboard::Key _goBackward,
				 const Keyboard::Key _fire)
			: turnLeft(_turnLeft), 
			  turnRight(_turnRight), 
			  goForward(_goForward), 
			  goBackward(_goBackward),
			  fire(_fire){}
		
		Keyboard::Key  turnRight = Keyboard::Unknown,
					    turnLeft = Keyboard::Unknown,
					   goForward = Keyboard::Unknown,
					  goBackward = Keyboard::Unknown,
							fire = Keyboard::Unknown;
	};*/

	// just moves the tank, missiles spawing etc is managed at a higher level
	void Update(float _elapsedTime);
	void Render(RenderWindow* _window);

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

	/*void 	setTankSpeed(float _tankSpeed) { tankSpeed = _tankSpeed; }
	float 	getTankSpeed() const { return tankSpeed; }
	void 	setTankAngleSpeed(float _tankAngleSpeed) { tankAngleSpeed = _tankAngleSpeed; }
	float 	getTankAngleSpeed() const { return tankAngleSpeed; }

	void    setInput(Inputmap _imap) { imap = _imap; }*/

private:
	int health;
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


	/*float tankSpeed;
	float tankAngleSpeed;*/

	const float fireCooldown = 0.1f;
	Clock fireClock;
	//Inputmap imap;

	void UpdateUserInput();
	bool Fire();
};
