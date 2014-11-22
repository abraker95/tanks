#pragma once
#define MAX_COMPONENTS_TYPE 128

#include <cstdlib>

template<typename T>
struct ComponentEntry 
{
	int entity_id;		
	T data;
	static int handle;
};

template<typename T>
int ComponentEntry<T>::handle;

class Memory
{
	
public:
	Memory() : num_components(1) { }
	~Memory()
	{
		for(int i=0;i<num_components;i++)
			free(component_tables[i]);
	}

	template<class T>
	int alloc(int num_entries)
	{
		void* table = malloc(sizeof(ComponentEntry<T>) * num_entries);
		component_tables[num_components] = table;
		ComponentEntry<T>::handle = num_components;

		for(int i=0;i<num_entries;i++)
			((ComponentEntry<T>*)table)->entity_id = -1;

		return num_components++;
	}
	
	template<class T>
	ComponentEntry<T>* access(int id)
	{
		return (ComponentEntry<T>*)component_tables[id];
	}

	void buildLookupTable(int num_entries)
	{
		size_t entry_size = sizeof(int) + sizeof(bool) * num_components;
		component_tables[0] = malloc(entry_size * num_entries);
	}

private:

	void* component_tables[MAX_COMPONENTS_TYPE];
	int num_components;
};
