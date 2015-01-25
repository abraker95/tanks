#pragma once
#include "core/environment.h"

class Name : public Component<Name>
{
public:
	Name(std::string name);

	std::string name;
};
