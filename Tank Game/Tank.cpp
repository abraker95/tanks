#include "Tank.h"

Tank::Tank(std::vector<Object*>* _objs)
	: Object(_objs, "Tank", 1) /// \NOTE: 1 frame for now
{
	// tank stats filled with the default values
	setHealth(100);
	setNumMines(0);
	setNumMissiles(0);
	setNumWalls(0);

	// tank special states
	setTankSpeed(0.f);
	setTankAngleSpeed(0.f);
}

Tank::~Tank()
{

}

/// \TODO: put this in the environment class, this way we can support multi
void Tank::UpdateUserInput()
{
	//PRINT_DEBUG
		 if(Keyboard::isKeyPressed(imap.turnRight))		tankAngleSpeed = 300.f;
	else if(Keyboard::isKeyPressed(imap.turnLeft))		tankAngleSpeed = -300.f;
	else												tankAngleSpeed = 0.f;

	     if(Keyboard::isKeyPressed(imap.goForward))		tankSpeed = 300.f;
	else if(Keyboard::isKeyPressed(imap.goBackward))	tankSpeed = -300.f;
	else												tankSpeed = 0.f;

	if(Keyboard::isKeyPressed(Keyboard::Space)&&Fire())
		addChildObject(new Bullet(getObjectList(), getPosition(), getRotation()+90));
}

bool Tank::Fire()
{
	if(fireClock.getElapsedTime().asSeconds() > fireCooldown)
	{
		fireClock.restart();
		return true;
	}

	return false;
}

void Tank::Update(float _elapsedTime)
{
	UpdateUserInput();

	float currAngle = getRotation();
	float nextAngle;

	const Vector2f& currPos = getPosition();
	Vector2f nextPos;

	nextAngle = currAngle + _elapsedTime * tankAngleSpeed;

	// (+PI/2) because of the sprite orientation
	nextPos.x = currPos.x + cosf(DEG2RAD(nextAngle) + PI/2) * tankSpeed * _elapsedTime;
	nextPos.y = currPos.y + sinf(DEG2RAD(nextAngle) + PI/2) * tankSpeed * _elapsedTime;
	
	setPosition(nextPos);
	setRotation(nextAngle);
}

void Tank::Render(RenderWindow* _window)
{
	Render(_window);
}
