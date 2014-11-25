#pragma once

struct Texture
{
	Texture();
	Texture(const char* filename);

	const char* filename;
	Texture* tex;
};
