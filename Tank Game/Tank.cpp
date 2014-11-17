#include "Tank.h"

Tank::Tank()
	: GameObject(true, "Tank", 1)
{
	// tank stats filled with the default values
	health = 100;
	maxHealth = 100;
	numMines = 0;
	numMissiles = 0;
	numWalls = 0;

	// tank special states
	speed = 0.f;
}

Tank::~Tank()
{

}

/// \TODO: put this in the environment class, this way we can support multi
void Tank::UpdateUserInput()
{
	//PRINT_DEBUG
		 if(Keyboard::isKeyPressed(imap.kmap[Turn_Right]))		angleSpeed = 300.f;
	else if(Keyboard::isKeyPressed(imap.kmap[Turn_Left]))		angleSpeed = -300.f;
	else														angleSpeed = 0.f;

	     if(Keyboard::isKeyPressed(imap.kmap[Go_Forward]))		speed = 300.f;
	else if(Keyboard::isKeyPressed(imap.kmap[Go_Backward]))		speed = -300.f;
	else														speed = 0.f;

	if(Keyboard::isKeyPressed(imap.kmap[Fire_Bullet]) && Fire())
		Environment::getSingleton()->addObject(new Bullet(getPosition(), getRotation()+90, this));
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
	elapsedTime = _elapsedTime;

	UpdateUserInput();

	//float currAngle = getRotation();
	float nextAngle = getRotation() + elapsedTime * angleSpeed;

	/*const Vector2f& currPos = getPosition();
	Vector2f nextPos;

	float nextAngle = currAngle+_elapsedTime * angleSpeed;
	nextAngle ;

	// (+PI/2) because of the sprite orientation
	nextPos.x = currPos.x + cosf(DEG2RAD(nextAngle) + PI/2) * speed * _elapsedTime;
	nextPos.y = currPos.y + sinf(DEG2RAD(nextAngle) + PI/2) * speed * _elapsedTime;*/
	
	setPosition(getPosition() + (Vector2f)getVelocityVector());
	setRotation(nextAngle);
}
