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
		 const type_info type;
		 Event_Type eventType;
		 T* var;
	 };

	 static std::vector<AbstVar<void*>*> vars;


	EventMsgr();
	virtual ~EventMsgr();

	template<class T> static void createEvent(Event_Type _event, T _var)
	{
		AbstVar<void*>* abstrVar = new AbstVar<void*>;
		abstrVar->eventType = _event;
		abstrVar->var = (void*)_var;
		abstrVar->type = typeid(_var);
	
		vars.push_back(abstrVar);
	}

	template<class T> static AbstVar<T>* getEvent(Event_Type _event)
	{
		AbstVar<T>* varRet;
		for(int i = 0; i<vars.size(); i++)
		{
			if(_event==OBJ_CREATE && vars[i]->type==typeid(Object))
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

