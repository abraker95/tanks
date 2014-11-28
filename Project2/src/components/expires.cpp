#include "components/expires.h"

Expires::Expires()
{}

Expires::Expires(float _time): time(_time), condition(false)
{}

Expires::Expires(bool& _cond) : time(-100000), condition(_cond)
{}

Expires::Expires(float _time, bool& _cond) : condition(_cond), time(_time)
{}
