#pragma once
#include <SFML/Graphics.hpp>
#include "Object.h"

class Tank : public Object
{
public:
	Tank(const char* filename, const Vector2f _gunOffset);
	virtual ~Tank();

	// just moves the tank, missiles spawing etc is managed at a higher level
	void Update(const float elapsedTime);

	// angle-based rendering
	void Render(RenderWindow* _window);

	// getters and setters
	// putting the defintion so that the compiler can inline them
	void 	setHealth(const int _health) 			{ health = _health; }
	int 	getHealth() const 						{ return health; 	}
	void 	setNumMines(const int _numMines)		{ numMines = _numMines; }
	int 	getNumMines() const						{ return numMines; 		}
	void 	setNumMissiles(const int _numMissiles) 	{ numMissiles = _numMissiles; 	}
	int 	getNumMissiles() const					{ return numMissiles; 			}
	void 	setNumWalls(const int _numWalls)		{ numWalls = _numWalls;	}
	int 	getNumWalls() const						{ return numWalls; 		}

	void 	setTankSpeed(float _tankSpeed)				{ tankSpeed = _tankSpeed; 	}
	float 	getTankSpeed() const						{ return tankSpeed; 		}
	void 	setTankAngle(float _tankAngle)				{ tankAngle = _tankAngle;	}
	float 	getTankAngle() const						{ return tankAngle; 		}
	void 	setGunAngle(float _gunAngle)				{ gunAngle = _gunAngle;	}
	float 	getGunAngle() const							{ return gunAngle;		}
	void 	setTankAngleSpeed(float _tankAngleSpeed)	{ tankAngleSpeed = _tankAngleSpeed;	}
	float 	getTankAngleSpeed() const					{ return tankAngleSpeed; 			}
	void 	setGunAngleSpeed(float _gunAngleSpeed)		{ gunAngleSpeed = _gunAngleSpeed;	}
	float 	getGunAngleSpeed() const					{ return gunAngleSpeed;				}


private:
	int health;
	int numMissiles;
	int numMines;
	int numWalls;

	float tankSpeed;
	float gunAngle;
	float tankAngle;
	float gunAngleSpeed;
	float tankAngleSpeed;

	Vector2f gunOffset;
};
