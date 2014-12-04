#include <iostream>
#include "systems/texture_manager.h"
#include "components/bounding_circle.h"
#include "components/texture.h"
#include "components/sprite.h"

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
	TextureHandle* tex_desc = env->get<TextureHandle>();
	Sprite* sprites = env->get<Sprite>();

	for(unsigned i=0;i<env->maxEntities();i++)
	{
		if(env->hasComponents<TextureHandle>(i))
		{
			sf::Texture* tex = lookupAtlas(tex_desc[i].filename);
			env->removeComponents<TextureHandle>(i);
			if(tex)
			{
				env->addComponents<Texture>(i, Texture(tex));

				sf::Vector2u size = tex->getSize();

				if(env->hasComponents<Sprite>(i))
					sprites[i].sprite.setOrigin((float)size.x/2.f, (float)size.y/2.f);	

				if(env->hasComponents<BoundingCircle>(i))
				{
					BoundingCircle* bounding_circle = env->get<BoundingCircle>();
					bounding_circle[i].radius = size.x < size.y ? (float)size.x/2.f : (float)size.y/2.f;
				}
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
