#include "framework.h"

Framework::Framework() : num_systems(0)
{
	memory = new Memory();
	memory->alloc<bool>(MAX_SYSTEM);
}

Framework::~Framework()
{
	if(memory)
		delete memory;
}

Memory* Framework::getMemory() const
{
	return memory;
}

int Framework::queueSystem(System system)
{
	ComponentEntry<bool>* systems_state = memory->access<bool>(1);
	systems[num_systems] = system;
	systems_state[num_systems].entity_id = 0;
	systems_state[num_systems].data = true;

	return num_systems++;
}

void Framework::run()
{
	ComponentEntry<bool>* systems_state = memory->access<bool>(1);

	bool systems_running;
	do
	{
		systems_running = false;
		for(int i=0;i<num_systems;i++)
		{
			if(systems_state[i].data)
			{
				(*systems[i])(memory, 0.f);
				systems_running = true;
			}
		}

	}while(systems_running);
}
