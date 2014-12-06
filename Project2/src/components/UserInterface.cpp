#include "components\UserInterface.h"

/// \TODO: Test this for crash
UserInterface::UserInterface(std::function<void*()> *_action, std::bitset<UIstates> _enable) : action(_action), cursorOnThis(false), enable(_enable) {}
