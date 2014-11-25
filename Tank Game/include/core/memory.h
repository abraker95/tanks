#pragma once

#include <cstdlib>
#include <tuple>
#include <bitset>

template<typename... Ts>
struct Entity
{
public:
	std::bitset<sizeof...(Ts)> mask; 
};

template<size_t size, typename... Ts>
class Environment
{
public:
	Environment() {}
	~Environment() {}

	template<typename T>
	T* get()
	{
		return &std::get<ComponentIndexByType<T, std::tuple<Ts...>>::value>(components)[0];
	}

	template<typename T>
	std::bitset<sizeof...(Ts)> getMask()
	{
		std::bitset<sizeof...(Ts)> mask;
		mask.set(ComponentIndexByType<T, std::tuple<Ts...>>::value);
		return mask;
	}

	template<typename T, typename... Rest>
	std::bitset<sizeof...(Ts)> getMask()
	{
		std::bitset<sizeof...(Ts)> mask = getMask<Rest...>();
		mask.set(ComponentIndexByType<T, std::tuple<Ts...>>::value);
		return mask;
	}

	int requestEntityID()
	{
		for(size_t i=0;i<size;i++)
		{
			if(entities[i].mask.none())
				return (int)i;
		}
		return -1;
	}

	template<typename T>
	void addComponents(int id, T&& t)
	{
		T* table = get<T>();

		table[id] = t;
		entities[id].set(ComponentIndexByType<T, std::tuple<Ts...>>::value);
	}

	template<typename T, typename... Rest>
	void addComponents(int id, T&& t, Rest&&... rest)
	{
		addComponents<T>(id, std::forward<T>(t));
		addComponents<Rest...>(id, std::forward<Rest>(rest)...);
	}

	template<typename T>
	void removeComponents(int id)
	{
		entities[id].set(ComponentIndexByType<T, std::tuple<Ts...>>::value, false);
	}

	template<typename T, typename... Rest>
	void removeComponents(int id)
	{
		removeComponents<T>(id);
		removeComponents<Rest...>(id);
	}

	std::array<Entity<Ts...>, size> entities;

private:
	std::tuple<std::array<Ts ,size>...> components;	// very big!

	template<class T, class Tuple>
	struct ComponentIndexByType
	{
	};
	
	template<class T, class... Types>
	struct ComponentIndexByType<T, std::tuple<T, Types...>>
	{
		static const std::size_t value = 0;
	};

	template<class T, class U, class... Types>
	struct ComponentIndexByType<T, std::tuple<U, Types...>>
	{
		static const std::size_t value = 1 + ComponentIndexByType<T, std::tuple<Types...>>::value;
	};
};
