#pragma once
#include "core/environment.h"

class SpawnLocation : public Component<SpawnLocation>
{
public:
	SpawnLocation();

	bool occupied;
};
