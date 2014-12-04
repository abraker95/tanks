#include "components\UserInterface.h"

/// \TODO: Test this for crash
UserInterface::UserInterface()/*: action([]()->void* { return nullptr; })*/ {}
UserInterface::UserInterface(std::function<void*()> *_action, std::bitset<UIstates> _enable) : action(_action), cursorOnThis(false), enable(_enable) {}

UserInterface::~UserInterface(){}