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
	     if(Keyboard::isKeyPressed(Keyboard::Left))		tankAngleSpeed = 1000.f;
	else if(Keyboard::isKeyPressed(Keyboard::Right))	tankAngleSpeed = -1000.f;
	else												tankAngleSpeed = 0.f;

	     if(Keyboard::isKeyPressed(Keyboard::Up))		tankSpeed = 1000.f;
	else if(Keyboard::isKeyPressed(Keyboard::Down))		tankSpeed = -1000.f;
	else												tankSpeed = 0.f;

	//  Ok, now I'm to the point where I need to make the envorinment class to manage all the objects. Also:
	/// \TODO: Delete the bullets at some point before the game exists
	/// \TODO: Bullet spawn position to come from tank's cannon. Note: the sprite's dimensions are likely to change later 
	/// [FIXED] \TODO: Fix the spammng of a million bullets under a tap of a button
	if(Keyboard::isKeyPressed(Keyboard::Space)) spawnObject(new Bullet(getPosition(), getRotation()));
}

// The movement is hilarius. Ok time for some fixing
void Tank::Update(RenderWindow* _window, float _elapsedTime)
{
	float currAngle = getRotation();
	float nextAngle;

	const Vector2f& currPos = getPosition();
	Vector2f nextPos;

	nextAngle = currAngle + _elapsedTime * tankAngleSpeed;

	nextPos.x = currPos.x + cosf(nextAngle) * tankSpeed * _elapsedTime;
	nextPos.y = currPos.y + sinf(nextAngle) * tankSpeed * _elapsedTime;


	setPosition(nextPos);
	setRotation(nextAngle);

	UpdateChildObjs(_window, _elapsedTime);
	Render(_window);
}
