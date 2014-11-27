#include <iostream>
#include "systems/texture_manager.h"

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
	for(auto it=texture_atlas.begin();it!=texture_atlas.end();it++)
	{
		delete it->second;
	}

	texture_atlas.clear();
}

void TextureManager::update(Environment* env)
{
	TextureHandle* texs = env->get<TextureHandle>();
	RenderProperties* props = env->get<RenderProperties>();

	for(unsigned i=0;i<env->maxEntities();i++)
	{
		if(env->hasComponents<TextureHandle>(i))
		{
			sf::Texture* tex = lookupAtlas(texs[i].filename);
			env->removeComponents<TextureHandle>(i);
			if(tex)
			{
				env->addComponents<RenderProperties>(i, RenderProperties(tex));
				sf::Vector2u size = tex->getSize();
				props[i].sprite.setOrigin((float)size.x/2.f, (float)size.y/2.f);	
			}
		}
	}
}

sf::Texture* TextureManager::lookupAtlas(const std::string& filename)
{
	auto it = texture_atlas.find(filename);
	if(it != texture_atlas.end())
		return it->second;
	
	sf::Texture* new_texture = new sf::Texture;
	if(!new_texture->loadFromFile(filename))
	{
		std::cout<<"ERROR: Could not load "<<filename<<std::endl;
		return nullptr;
	}
	else
		std::cout<<"SUCCESS: Loaded "<<filename<<" successfully"<<std::endl;
	
	texture_atlas[filename] = new_texture;
	return new_texture;
}
