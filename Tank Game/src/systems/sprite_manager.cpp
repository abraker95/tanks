#include "systems/sprite_manager.h"

void SpriteManager::update(Environment* env, float)
{
	TextureHandle* texs = env->get<TextureHandle>();

	for(unsigned i=0;i<env->maxEntities();i++)
	{
		if(env->hasComponents<TextureHandle>(i))
		{
			if(texs[i].texture == nullptr)
			{
				// load file
			}
		}
	}
}
