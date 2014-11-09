#pragma once

#define MIN(_a, _b) ((_a < _b)? _a: _b)
#define MAX(_a, _b) ((_a > _b)? _a: _b)
#define BTWN(_a, _x, _b) ((min(_a, _b) <= _x) && (_x <= max(_a, _b)))
