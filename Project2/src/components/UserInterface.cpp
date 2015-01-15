#include "components\UserInterface.h"

/// \TODO: Test this for crash
UserInterface::UserInterface()/*: action([]()->void* { return nullptr; })*/ {}
UserInterface::UserInterface(std::bitset<UIstates> _enable, std::function<void*()> _action, short _subMenu): action(new std::function<void*()>(_action)), cursorOnThis(false), enable(_enable), show(true), subMenu(_subMenu) {}

UserInterface::~UserInterface() { if(action != nullptr) delete action; }