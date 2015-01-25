#pragma once

#include <SFML/System/Clock.hpp>
#include <cstdio>
#include <cstdlib>
#include <bitset>
#include <map>
#include <array>
#include <vector>
#include <iostream>
#include <typeinfo>
#include "DEBUG.h"

#define MAX_COMPONENTS 64
#define MAX_EVENTS 64
#define MAX_SYSTEMS 64

typedef std::bitset<MAX_COMPONENTS> ComponentMask;

class ComponentBase;
template<typename T> class Component;
template<typename T> class ComponentIterator;
class Environment;

class EventBase;
template<typename T> class Event;
template<typename T> class EventIterator;

class System;

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

// \TODO: Make the constructor use better
template<typename T>
struct StdComponent: public Component<T>
{
	public:
		StdComponent(T* _data, std::string _name = "") { data = _data; name = _name;  del = true; }
		StdComponent(std::string _name = "") { data = nullptr; name = _name;  del = false; }
		virtual ~StdComponent() { if(del) delete data; }

		void set(T* _data) { if(data) delete data;  data = _data; del = false; }

		T* data;
		std::string name;
		
	private:
		bool del;
};

#define Component(type, name, val) new StdComponent<type>(val, name)
//#define Component(type, val) new StdComponent<type>(val)

// Same concept as components but for events
class EventBase 
{
	friend class Environment;
	void* emitter;

	public:
		virtual ~EventBase() {}
};

inline unsigned getEventID()
{
	static unsigned counter = 0;
	return counter++;
}

template<typename T>
class Event : public EventBase
{
	friend class Environment;

	static unsigned bitpos() 
	{
		static unsigned id = getEventID();
		return id;
	}

	public:
		virtual ~Event() {}
		
};

class System
{
public:
	float sum = 0.f;
	int num_it = 0;
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
		entityName.resize(num_entities);
	}

	virtual ~Environment() 
	{
		for(unsigned i=0;i<components.size();i++)
			delete components[i];
	}

	template<typename T>
	ComponentIterator<T> get()
	{
		return ComponentIterator<T>(this);
	}

	// request a free entity id, and attach components to it
	template<typename... T>
	unsigned createEntity(std::string _name, T*... t)
	{
		unsigned new_id = requestID();
		addComponents<T...>(new_id, t...);
		
		PRINT_DEBUG(std::cout<<" Create Entity: "<<new_id<<"    Name: "<<_name<<std::endl, MED_DEBUG, ENVSYS);

		entityName[new_id] = _name;
		return new_id;
	}

	// set the componentmask to 0 and make the name blank
	void destroyEntity(unsigned id)
	{
		deleteID(id);
		entityName[id] = "";
	}

	// returns 1<<Component<T>::bitpos
	template<typename T>
	ComponentMask getMask()
	{
		ComponentMask mask;
		mask.set(Component<T>::bitpos());
		return mask;
	}

	std::string getEntityName(unsigned _id)
	{
		// \TODO: Add invalid ref check
		return entityName[_id];
	}

	int getID(std::string _entityName)
	{
		for(unsigned int i = 0; i<entityName.size(); i++)
			if(entityName[i]==_entityName) return i;
		
		cout<<"[ENV]: An entity with the name \""<<_entityName<<"\" has not been found"<<endl; /// \NOTE (abraker): To avoid hours of misery due to a mispelling
		return 0;
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
		std::string* Nameptr = (std::string*)components_pointer[id][Component<T>::bitpos()];
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

	template<typename T>
	void disableComponents(unsigned id)
	{
		entity_mask[id] &= ~getMask<T>();
	}

	template<typename T, typename R, typename... Rs>
	void disableComponents(unsigned id)
	{
		disableComponents<T>(id);
		disableComponents<R, Rs...>(id);
	}

	template<typename T>
	void enableComponents(unsigned id)
	{
		entity_mask[id] |= getMask<T>();
	}
	
	// allow copying multiple components for an entity at once
	template<typename T, typename R, typename... Rs>
	void enableComponents(unsigned id)
	{
		enableComponents<T>(id);
		enableComponents<R, Rs...>(id);
	}

	// does an AND operation between the entity's mask and the requested components
	// mask and checks if the entity has all the requested components
	template<typename... T>
	bool hasComponents(unsigned id)
	{
		if(entity_mask[id].none()) 
			return false;

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
		return *(dynamic_cast<T*>(components_pointer[id][Component<T>::bitpos()]));
	}

	template<typename T>
	void emit(T* t)
	{
		//PRINT_DEBUG(cout<<"[SET] Emmiter: "<<current_system<<endl, HI_DEBUG, GFXSYS);
		int id = Event<T>::bitpos();
		t->emitter = current_system;
		events_queue[id].push_back(t);
	}

	template<typename T>
	EventIterator<T> getEvents()
	{
		return EventIterator<T>(this);
	}

	template<typename T>
	unsigned numEvents()
	{
		//PRINT_DEBUG(cout<<"menuHides size: "<<events_queue[Event<T>::bitpos()].size()<<endl, HI_DEBUG, GFXSYS);
		return events_queue[Event<T>::bitpos()].size();
	}

	template<typename T>
	T* getEvent(unsigned index)
	{
	//	int id = Event<T>::bitpos();
	//	eventExec[id] = true;
		return static_cast<T*>(events_queue[Event<T>::bitpos()][index]);
	}

	const std::string& resetMonitoring(float elapsed)
	{
		static float sum_elapsed = 0.f;
		static int num_it = 0;

		sum_elapsed += elapsed;
		num_it++;

		if(refresh_avg <= 0.f)
		{
			refresh_avg = refresh_avg_cooldown;
			monitoring_results = next_monitoring_results;
		}
		refresh_avg -= elapsed;

		if(refresh_avg <= 0.f)
		{
			next_monitoring_results = "";
			float avg_elapsed = sum_elapsed/(float)num_it;
			next_monitoring_results += "Elapsed time : ";
			next_monitoring_results += std::to_string((int)avg_elapsed);
			next_monitoring_results += " us\t\t\t";
			next_monitoring_results += std::to_string((int)(1.f/avg_elapsed));
			next_monitoring_results += " FPS\n\n";

			sum_elapsed = 0.f;
			num_it = 0;
		}	

		return monitoring_results;
	}

	// wrapper function
	template<typename T, typename... Args>
	void updateWrapper(T* sys, Args&&... args)
	{
		static sf::Clock clock;

		current_system = reinterpret_cast<void*>(sys);
		clearEvents(current_system);

		clock.restart();
		sys->update(this, args...);
		sf::Time elapsed = clock.getElapsedTime();

		sys->sum += elapsed.asMicroseconds();
		sys->num_it++;

		if(refresh_avg <= 0.f)
		{
			next_monitoring_results += "[";
			next_monitoring_results += typeid(T).name();
			next_monitoring_results += "]\t\t\tElapsed time : ";
			next_monitoring_results += std::to_string((int)(sys->sum/(float)sys->num_it));
			next_monitoring_results += " us\n";

			sys->sum = 0.f;
			sys->num_it = 0;
		}
	}

	void clearEvents(void* emitter)
	{
	//	PRINT_DEBUG(cout<<"Clear Events: "<<endl, HI_DEBUG, GFXSYS);
		for(unsigned i=0;i<MAX_EVENTS;i++)
		{
			//PRINT_DEBUG(cout<<" eventExec: "<<eventExec[i]<<"      events_queue: "<<i<<endl, HI_DEBUG, ENVSYS);
			for(unsigned j = 0; j<events_queue[i].size(); j++)
			{
				if(events_queue[i][j]->emitter == emitter)
				{
					//PRINT_DEBUG(cout<<"\t[CLEAR] Emmiter: "<<emitter<<endl, HI_DEBUG, GFXSYS);
					delete events_queue[i][j];
					events_queue[i].erase(events_queue[i].begin() + j);
				}

				else
				{
					j++;
				}
			}
		}
	}

/*
public:

	// Note(ABraker): I hope this solution is good enough. Only 
	// application can touch managers, and this is the only 
	// workaround I can think of.
	sf::Text* cpuData;
*/

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
	std::vector<std::string> entityName;
	std::vector<std::array<ComponentBase*, MAX_COMPONENTS>> components_pointer;
	std::array<std::vector<EventBase*>, MAX_EVENTS> events_queue;

	std::string monitoring_results, next_monitoring_results;
	float refresh_avg = 0.f;
	const float refresh_avg_cooldown = 0.1f;

	void* current_system;
};

// The iterator is a wrapper class for the environment component getter
template<typename T>
class ComponentIterator
{
public:
	ComponentIterator(Environment* env) : env(env) {}

	T& operator[](unsigned entity_id)
	{
		return env->getComponent<T>(entity_id);
	}

private:
	Environment* env;
};

// same thing for events
template<typename T>
class EventIterator
{
public:
	EventIterator(Environment* env) : env(env) {}

	T& operator[](unsigned index)
	{
		return *(env->getEvent<T>(index));
	}

	unsigned size()
	{	
		return env->numEvents<T>();
	}

private:
	Environment* env;
};
