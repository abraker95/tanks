#pragma once

#include "core/environment.h"

struct MapDesc : public Component<MapDesc>
{
	MapDesc(std::string mapfile);
	
	std::string mapfile;
};
