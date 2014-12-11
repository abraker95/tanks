#pragma once

#include <cstdio>
#include <cstdlib>
#include <bitset>
#include <map>
#include <array>
#include <vector>
#include <iostream>
#include "DEBUG.h"

#define MAX_COMPONENTS 64
#define MAX_EVENTS 64

typedef std::bitset<MAX_COMPONENTS> ComponentMask;

class ComponentBase;
template<typename T> class Component;
template<typename T> class Iterator;
class Environment;

class EventBase;
template<typename T> class Event;

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


// Same concept as components but for events
class EventBase 
{
public:
	virtual ~EventBase() {}
};

inline unsigned getEventID()
{
	static unsigned counter = 0;
	return counter++;
}

template<typename T>
class Event
{
	friend class Environment;
	static unsigned bitpos() 
	{
		static unsigned id = getEventID();
		return id;
	}

public:
	Event() {}
	virtual ~Event() {}
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

	template<typename T>
	void emit(T&& t)
	{
		events_queue[Event<T>::bitpos()].push_back(std::forward(t));
	}

	template<typename T>
	const std::vector<T>* getEvents()
	{
		return &events_queue[Event<T>::bitpos()];
	}

	void clearEvents()
	{
		for(unsigned i=0;i<MAX_EVENTS;i++)
			events_queue[i].clear();
	}

private:
	unsigned requestID()
	{
		for(unsigned i=1;i<maxEntities();i++)
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
	std::array<std::vector<EventBase>, MAX_EVENTS> events_queue;
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
