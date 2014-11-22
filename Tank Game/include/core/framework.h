#pragma once
#include "memory.H"

#define MAX_SYSTEM 128

typedef void (*System)(Memory*, float);

class Framework
{
public:
	Framework();
	~Framework();

	Memory* getMemory() const;
	int queueSystem(System system);
	void run();

private:
	Memory* memory;
	System systems[MAX_SYSTEM];
	int num_systems;
};
