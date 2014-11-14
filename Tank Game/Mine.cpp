#include "Mine.h"

Mine::Mine() : GameObject(false, "Mine", 1)
{
	detonated = false;
	explosionProgress = false;
	explosionDuration = 0.3f;
	blowRadius = 50.f;
}

Mine::~Mine()
{
}

void Mine::Detonate()
{
	detonated = true;
}

void Mine::Update(float _elapsedTime)
{
	if(detonated)
	{
		if(explosionProgress > 1.f)
		{
			destroy();
		}

		else
		{
			explosionProgress += _elapsedTime/explosionDuration;
			boundingCircleRadius = explosionProgress * blowRadius;
		}
	}
}
