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

class ComponentBase;
template<typename T> class Component;
class Environment;
template<typename T> class Iterator;

class ComponentBase
{
public:
	virtual ~ComponentBase() {}
};

/// \TODO: Create environment variables in the game for settings/options. 
///		   For example: Option to control tank's speed. A system will check
///		   and apply the updated values to the entity's local vars

inline unsigned getComponentID()
{
	static unsigned counter = 0;
	return counter++;
}

// every component needs to inherit from this class
// for every new component bitpos will be assigned with a new unique value
template<typename T>
class Component : public ComponentBase
{
	friend class Environment;
	static unsigned bitpos() 
	{
		static unsigned id = getComponentID();
		return id;
	}

public:
	Component() {}
	virtual ~Component() {}
};

// the Environment class holds the entities' informations
// and a table of pointers to the components arrays
// this system can only have *one* array for a component type
class Environment
{
public:
	Environment(int num_entities) 
	{
		entity_mask.resize(num_entities);
		components_pointer.resize(num_entities);
	}

	virtual ~Environment() 
	{
		for(unsigned i=0;i<components.size();i++)
			delete components[i];
	}

	template<typename T>
	Iterator<T> get()
	{
		return Iterator<T>(this);
	}

	// request a free entity id, and attach components to it
	template<typename... T>
	unsigned createEntity(T*... t)
	{
		PRINT_DEBUG(std::cout<<" Create Entity"<<std::endl, MED_DEBUG, ENVSYS);

		unsigned new_id = requestID();
		addComponents<T...>(new_id, t...);

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
		mask.set(Component<T>::bitpos());
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
	void addComponents(unsigned id, T* init)
	{
		entity_mask[id] |= getMask<T>();

		components_pointer[id][Component<T>::bitpos()] = init;
		components.push_back(init);
	}
	
	// allow copying multiple components for an entity at once
	template<typename T, typename R, typename... Rs>
	void addComponents(unsigned id, T* init, R* r, Rs*... rs)
	{
		addComponents<T>(id, init);
		addComponents<R, Rs...>(id, r, rs...);
	}

	// remove the bit which tells that the entity ( id ) has the component ( T )
	template<typename T>
	void removeComponents(unsigned id)
	{
		entity_mask[id] &= ~getMask<T>();

		ComponentBase* ptr = components_pointer[id][Component<T>::bitpos()];
		for(unsigned i=0;i<components.size();i++)
		{
			if(components[i] == ptr)
			{
				components.erase(components.begin()+i);
				delete ptr;
				break;
			}
		}
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
		return (entity_mask[id] & mask) == mask;
	}

	// returns the max number of entities
	unsigned maxEntities()
	{
		return entity_mask.size();
	}

	template<typename T>
	T& getComponent(unsigned id)
	{
		return *(static_cast<T*>(components_pointer[id][Component<T>::bitpos()]));
	}

private:
	unsigned requestID()
	{
		for(unsigned i=0;i<maxEntities();i++)
		{
			if(entity_mask[i].none())
				return i;
		}

		// error
		PRINT_DEBUG(std::cout<<" Too many entities!"<<std::endl, MED_DEBUG, ENVSYS);
		return 0;
	}

	void deleteID(unsigned id)
	{
		entity_mask[id].reset();
	}

private:
	std::vector<ComponentBase*> components;
	std::vector<ComponentMask> entity_mask;
	std::vector<std::array<ComponentBase*, MAX_COMPONENTS>> components_pointer;
};

// The iterator is a wrapper class for the environment component getter
template<typename T>
class Iterator
{
public:
	Iterator(Environment* env) : env(env) {}

	T& operator[](unsigned entity_id)
	{
		return env->getComponent<T>(entity_id);
	}

private:
	Environment* env;
};
