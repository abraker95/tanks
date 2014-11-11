#include "Tank.h"

Tank::Tank()
	: Object("Tank", 1) /// \NOTE: 1 frame for now
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

void Tank::UpdateUserInput()
{
	// constant speed so it doesnt go craz^y when the key is pressed for a long time
	     if(Keyboard::isKeyPressed(Keyboard::Right))	tankAngleSpeed = 300.f;
	else if(Keyboard::isKeyPressed(Keyboard::Left))		tankAngleSpeed = -300.f;
	else												tankAngleSpeed = 0.f;

	     if(Keyboard::isKeyPressed(Keyboard::Up))		tankSpeed = 300.f;
	else if(Keyboard::isKeyPressed(Keyboard::Down))		tankSpeed = -300.f;
	else												tankSpeed = 0.f;

	//  Ok, now I'm to the point where I need to make the envorinment class to manage all the objects. Also:
	/// \TODO: Delete the bullets at some point before the game exists
}

bool Tank::Fire(float _cooldown)
{
	static Clock clock;

	if(clock.getElapsedTime().asSeconds() < _cooldown)
	{
		clock.restart();
		return true;
	}

	return true;
}

// The movement is hilarius. Ok time for some fixing
void Tank::Update(RenderWindow* _window, float _elapsedTime)
{
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

	Render(_window);
}
