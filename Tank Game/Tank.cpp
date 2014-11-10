#include <cmath>
#include "Tank.h"

Tank::Tank(const char* _filename)
	: Object(_filename, 1) // NOTE: 1 frame for now
{
	// tank stats filled with the default values
	setHealth(100);
	setNumMines(0);
	setNumMissiles(0);
	setNumWalls(0);

	// tank spacial states
	setTankSpeed(0.f);
	setTankAngleSpeed(0.f);
}

Tank::~Tank()
{

}

void Tank::Update(float elapsedTime)
{
	float currAngle = getRotation();
	float nextAngle;

	const Vector2f& currPos = getPosition();
	Vector2f nextPos;

	nextAngle = currAngle + elapsedTime * tankAngleSpeed;

	nextPos.x = currPos.x + cosf(nextAngle) * tankSpeed * elapsedTime;
	nextPos.y = currPos.y + sinf(nextAngle) * tankSpeed * elapsedTime;


	setPosition(nextPos);
	setRotation(nextAngle);
}
