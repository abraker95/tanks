#include <cmath>
#include "Tank.h"

Tank::Tank(const char* _filename, Vector2f _gunOffset)
	: Object(_filename, 2), gunOffset(_gunOffset) // the tank body and the gun
{
	// tank stats filled with the default values
	setHealth(100);
	setNumMines(0);
	setNumMissiles(0);
	setNumWalls(0);

	// tank spacial states
	setTankSpeed(0.f);
	setTankAngle(0.f);
	setGunAngle(0.f);
	setTankAngleSpeed(0.f);
	setGunAngleSpeed(0.f);
}

Tank::~Tank()
{

}

void Tank::Update(const float elapsedTime)
{
	const Vector2f& currPos = getPosition();
	Vector2f nextPos;

	tankAngle += elapsedTime * tankAngleSpeed;
	gunAngle += elapsedTime * gunAngleSpeed;

	nextPos.x = currPos.x + cosf(tankAngle) * tankSpeed * elapsedTime;
	nextPos.y = currPos.y + sinf(tankAngle) * tankSpeed * elapsedTime;

	setPosition(nextPos);
}

void Tank::Render(RenderWindow* _window)
{
	const Vector2f& bodyPosition = getPosition();
	Vector2f gunPosition = gunOffset + bodyPosition;

	// renders the body of the tank
	setRotation(tankAngle);
    setTexture(texture[0]);
	_window->draw(*this);

	// renders the gun
	setRotation(gunAngle);
	setPosition(gunPosition);
    setTexture(texture[1]);
	_window->draw(*this);

	// reset the bodyPosition
	setPosition(bodyPosition);
}
