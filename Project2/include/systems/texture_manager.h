#pragma once

#include <string>
#include <map>
#include "core/environment.h"
#include "components/texture_handle.h"
#include "components/transform.h"
#include "components/render_properties.h"

class TextureManager
{
public:
	TextureManager();
	~TextureManager();

	void update(Environment* env);
	sf::Texture* lookupAtlas(const std::string& filename);

private:
	std::map<std::string, sf::Texture*> texture_atlas;
};
