#pragma once

#include <cstdio>
#include <cstdlib>
#include <bitset>
#include <map>
#include <vector>
#include <iostream>

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
	Environment(const int num_entites) 
	{
		entities.resize(num_entites);
		for(unsigned i=0;i<MAX_COMPONENTS;i++)
			components_table[i] = nullptr;
	}

	virtual ~Environment() 
	{
	}

	template<typename T>
	T* get()
	{
		return static_cast<T*>(components_table[Component<T>::bitpos]);
	}

	template<typename T>
	void alloc()
	{
		void* pool = (void*)(new T[entities.size()]);
		components_table[Component<T>::bitpos] = pool;
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
		components_table[Component<T>::bitpos] = nullptr;

		delete[] static_cast<T*>(pool);
	}

	template<typename T, typename U, typename... Rest>
	void dealloc()
	{
		dealloc<T>();
		dealloc<U, Rest...>();
	}

	template<typename... T>
	void createEntity(T&&... t)
	{
		unsigned new_id = requestID();
		addComponents<T...>(new_id, std::forward<T>(t)...);
	}

	void destroyEntity(unsigned id)
	{
		deleteID(id);
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

	template<typename T>
	void addComponents(unsigned id, T&& init)
	{
		entities[id] |= getMask<T>();
		T* pool = get<T>();
		pool[id] = init;
	}

	template<typename T, typename R, typename... Rs>
	void addComponents(unsigned id, T&& init, R&& r, Rs&&... rs)
	{
		addComponents<T>(id, std::forward<T>(init));
		addComponents<R, Rs...>(id, std::forward<R>(r), std::forward<Rs>(rs)...);
	}

	template<typename T>
	void removeComponents(unsigned id)
	{
		entities[id] &= ~getMask<T>();
	}

	template<typename T, typename R, typename... Rs>
	void removeComponents(unsigned id)
	{
		removeComponents<T>(id);
		removeComponents<R, Rs...>(id);
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
