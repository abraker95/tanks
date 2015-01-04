#include "components\UserInterface.h"

/// \TODO: Test this for crash
UserInterface::UserInterface()/*: action([]()->void* { return nullptr; })*/ {}
UserInterface::UserInterface(std::bitset<UIstates> _enable, std::function<void*()> _action): action(new std::function<void*()>(_action)), cursorOnThis(false), enable(_enable), show(true) {}

UserInterface::~UserInterface() { if(action != nullptr) delete action; }