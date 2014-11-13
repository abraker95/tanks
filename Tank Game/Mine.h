#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"

class Mine : public GameObject
{
public:
	Mine();
	virtual ~Mine();

	void Update(float _elapsedTime);

	void Detonate();

private:
	bool detonated;
	float explosionProgress;
	float explosionDuration;
	float blowRadius;
};
