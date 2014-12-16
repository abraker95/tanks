#include "components/transform.h"

Transform::Transform(Vec2f pos, float rot): pos(pos), rot(rot), scale(Vec2f(1.f, 1.f))
{
}

Transform::Transform(Vec2f pos, Vec2f scale, float rot): pos(pos), rot(rot), scale(Vec2f(1.f, 1.f))
{
}
