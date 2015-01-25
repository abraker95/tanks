#pragma once
#include "core/environment.h"

class SpawnLocation : public Component<SpawnLocation>
{
public:
	SpawnLocation(int player_id);

	int player_id;
};
