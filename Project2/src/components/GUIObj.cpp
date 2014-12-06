#include "components/GUIObj.h"

GUIObj::GUIObj(Type _type, std::function<void*()> _action) : type(_type), action(_action) {}
