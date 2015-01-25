#pragma once

#include "core/environment.h"

class Player : public Component<Player>
{
public:
	Player(int player_id);

	int player_id;
};
