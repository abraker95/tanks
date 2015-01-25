#pragma once

#include "core/environment.h"
#include "managers/managers.h"
#include "events.h"

class DamageSystem : public System
{
	public:		
		void update(Environment* env, Managers* managers);

	private:
		void handleProjectiles(Environment* _gameEnv, const CollisionEvent& collision, Managers* managers);
};
