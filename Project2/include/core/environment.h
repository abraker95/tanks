#pragma once

#include <cstdio>
#include <cstdlib>
#include <bitset>
#include <map>
#include <vector>
#include <iostream>
#include "DEBUG.h"

#define MAX_COMPONENTS 64

typedef std::bitset<MAX_COMPONENTS> ComponentMask;

/// \TODO: Create environment variables in the game for settings/options. 
///		   For example: Option to control tank's speed. A system will check
///		   and apply the updated values to the entity's local vars

// every component needs to inherit from this class
// for every new component bitpos will be assigned with a new unique value
template<typename T>
struct Component
{
	static const unsigned bitpos;
};

static unsigned bitpos_index = 0;

// here bitpos get its value
template<typename T>
const unsigned Component<T>::bitpos = bitpos_index++;

// the Environment class holds the entities' informations
// and a table of pointers to the components arrays
// this system can only have *one* array for a component type
class Environment
{
public:
	Environment(const int num_entites) 
	{
		entities.resize(num_entites);
		for(unsigned i=0;i<MAX_COMPONENTS;i++)
			components_table[i] = nullptr;
	}

	virtual ~Environment() 
	{
	}

	// returns a pointer to the component arrays
	// the type T acts as a parameter
	template<typename T>
	T* get()
	{
		return static_cast<T*>(components_table[Component<T>::bitpos]);
	}

	// allocate a pool in the memory for a component
	template<typename T>
	void alloc()
	{
		void* pool = (void*)(new T[entities.size()]);
		components_table[Component<T>::bitpos] = pool;
	}

	// allow multiple Components to be allocated at once
	template<typename T, typename U, typename... Rest>
	void alloc()
	{
		alloc<T>();
		alloc<U, Rest...>();
	}

	// deallocate a pool in the memory for a component
	template<typename T>
	void dealloc()
	{
		void* pool = components_table[Component<T>::bitpos];
		components_table[Component<T>::bitpos] = nullptr;

		delete[] static_cast<T*>(pool);
	}

	// allow multiple Components to be deallocated at once
	template<typename T, typename U, typename... Rest>
	void dealloc()
	{
		dealloc<T>();
		dealloc<U, Rest...>();
	}

	// request a free entity id, and attach components to it
	// arguments are rvalue ref to allow this kind of function call:
	// createEntity(Transform())
	// and because it is "moved" around the copying is kept to the minimum.
	template<typename... T>
	unsigned createEntity(T&&... t)
	{
		PRINT_DEBUG(std::cout<<" Create Entity"<<std::endl, MED_DEBUG, ENVSYS);
		unsigned new_id = requestID();
		addComponents<T...>(new_id, std::forward<T>(t)...);

		return new_id;
	}

	// set the componentmask to 0
	void destroyEntity(unsigned id)
	{
		deleteID(id);
	}

	// returns 1<<Component<T>::bitpos
	template<typename T>
	ComponentMask getMask()
	{
		ComponentMask mask;
		mask.set(Component<T>::bitpos);
		return mask;
	}

	// ORing every 1<<Component<T>::bitpos
	template<typename T, typename U, typename... Rest>
	ComponentMask getMask()
	{
		ComponentMask mask = getMask<T>();
		return mask | getMask<U, Rest...>();
	}

	// copy a component ( T ), for the entity ( id )
	template<typename T>
	void addComponents(unsigned id, T&& init)
	{
		entities[id] |= getMask<T>();
		T* pool = get<T>();
		pool[id] = init;
	}
	
	// allow copying multiple components for an entity at once
	template<typename T, typename R, typename... Rs>
	void addComponents(unsigned id, T&& init, R&& r, Rs&&... rs)
	{
		addComponents<T>(id, std::forward<T>(init));
		addComponents<R, Rs...>(id, std::forward<R>(r), std::forward<Rs>(rs)...);
	}

	// remove the bit which tells that the entity ( id ) has the component ( T )
	template<typename T>
	void removeComponents(unsigned id)
	{
		entities[id] &= ~getMask<T>();
	}

	// allow removing multiple components for an entity ( id ) at once
	template<typename T, typename R, typename... Rs>
	void removeComponents(unsigned id)
	{
		removeComponents<T>(id);
		removeComponents<R, Rs...>(id);
	}

	// does an AND operation between the entity's mask and the requested components
	// mask and checks if the entity has all the requested components
	template<typename... T>
	bool hasComponents(unsigned id)
	{
		ComponentMask mask = getMask<T...>();
		return (entities[id] & mask) == mask;
	}

	// returns the max number of entities
	unsigned maxEntities()
	{
		return entities.size();
	}

private:
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

	void deleteID(unsigned id)
	{
		entities[id].reset();
	}

private:
	void* components_table[MAX_COMPONENTS];
	std::vector<ComponentMask> entities;
};
