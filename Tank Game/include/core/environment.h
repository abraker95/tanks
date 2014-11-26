#pragma once

#include <cstdio>
#include <cstdlib>
#include <bitset>
#include <map>
#include <vector>

#define MAX_COMPONENTS 32

typedef std::bitset<MAX_COMPONENTS> ComponentMask;

template<typename T>
struct Component
{
	static const unsigned bitpos;
};

static unsigned bitpos_index = 0;

template<typename T>
const unsigned Component<T>::bitpos = bitpos_index++;

class Environment
{
public:
	Environment(int num_entites) 
	{
		entities.resize(num_entites);
	}

	virtual ~Environment() 
	{
		for(std::map<unsigned, void*>::iterator it=components_table.begin();it!=components_table.end();++it)
			free(it->second);

		components_table.clear();
	}

	template<typename T>
	T* get()
	{
		return (T*)(components_table[Component<T>::bitpos]);
	}

	template<typename T>
	void alloc()
	{
		void* pool = malloc(sizeof(T) * entities.size());
		components_table[Component<T>::bitpos] = (void*)pool;
	}

	template<typename T, typename U, typename... Rest>
	void alloc()
	{
		alloc<T>();
		alloc<U, Rest...>();
	}


	template<typename T>
	void dealloc()
	{
		void* pool = components_table[Component<T>::bitpos];
		components_table.erase(Component<T>::bitpos);

		free(pool);
	}

	template<typename T, typename U, typename... Rest>
	void dealloc()
	{
		dealloc<T>();
		dealloc<U, Rest...>();
	}

	unsigned requestID()
	{
		for(unsigned i=0;i<entities.size();i++)
		{
			if(entities[i].none())
				return i;
		}

		// error
		return 0;
	}

	void deleteID(int id)
	{
		entities[id].reset();
	}

	template<typename T>
	ComponentMask getMask()
	{
		ComponentMask mask;
		mask.set(Component<T>::bitpos);
		return mask;
	}

	template<typename T, typename U, typename... Rest>
	ComponentMask getMask()
	{
		ComponentMask mask = getMask<T>();
		return mask | getMask<U, Rest...>();
	}

	template<typename... T>
	bool hasComponents(unsigned id)
	{
		ComponentMask mask = getMask<T...>();
		return (entities[id] & mask) == mask;
	}

	unsigned maxEntities()
	{
		return entities.size();
	}

private:
	std::map<unsigned, void*> components_table;
	std::vector<ComponentMask> entities;
};
