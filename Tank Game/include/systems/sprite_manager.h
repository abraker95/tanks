#pragma once

#include "core/environment.h"
#include "components/texture_handle.h"
#include "components/transform.h"

class SpriteManager
{
public:
	SpriteManager() { }
	void update(Environment* env, float dt);

private:
};
