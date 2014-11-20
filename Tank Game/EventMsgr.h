#pragma once

#include <typeinfo>
#include <vector>
//#include <csignal>

class EventMsgr
{
 public:
	 enum Event_Type
	 {
		 OBJ_CREATE = 0
	 };

	 template<class T>
	 struct AbstVar
	 {
		 AbstVar<T>();
		 const type_info* type;
		 Event_Type eventType;
		 T var;
	 };

	std::vector<AbstVar<void*>*> vars;


	EventMsgr();
	virtual ~EventMsgr();

	template<class T> void createEvent<T>(Event_Type _event, T _var)
	{
		/*vars.push_back(new AbstVar<void*>);
			vars[vars.size()-1]->eventType = _event;
			vars[vars.size()-1]->var	   = (void*)_var;
			vars[vars.size()-1]->type	   = &typeid(_var);*/
	}

	/// \TODO: AbstVar returned never gets deleted
	template<class T> AbstVar<T>* getEvent(Event_Type _event)
	{
		AbstVar<T>* varRet;
		for(int i = 0; i<vars.size(); i++)
		{
			if(_event==OBJ_CREATE && vars[i]->type==&typeid(Object))
			{
				varRet = (AbstVar<T>*)vars[i];
				vars.erase(vars.begin()+i);
				return varRet;
			}
		}
		return nullptr;
	}
};


extern EventMsgr event;

