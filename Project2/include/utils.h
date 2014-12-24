#pragma once

#define PI 3.14159f
#define MIN(_a, _b) ((_a < _b)? _a: _b)
#define MAX(_a, _b) ((_a > _b)? _a: _b)
#define BTWN(_a, _x, _b) ((MIN(_a, _b) <= _x) && (_x <= MAX(_a, _b)))
#define DEG2RAD(_a) (_a*(PI/180.f))
#define ABS(_x) ((_x < 0)? -_x: _x)
#define COND(_cond, _a, _b) (_cond? _a: _b)
#define LIMIT(_a, _x, _b) COND(_x>MAX(_a, _b), MAX(_a, _b), COND(_x<MIN(_a, _b), MIN(_a, _b), _x))
#define DELTA(_a, _b) (MAX(_a, _b) - MIN(_a, _b))
#define token2string(x) #x