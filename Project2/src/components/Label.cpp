#include "components/Label.h"

Label::Label(std::string _text): label(_text.data(), sf::Font(), _text.size()) { }

Label::~Label() {}
