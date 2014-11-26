#pragma once

#include "core/environment.h"

struct Texture : public Component<Texture>
{
	Texture();
	Texture(const char* filename);

	const char* filename;
	Texture* tex;
};
