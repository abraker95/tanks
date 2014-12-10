#include "components/bounding_circle.h"

BoundingCircle::BoundingCircle() : radius(0.f), collision(false)
{

}

BoundingCircle::BoundingCircle(float radius) : radius(radius), collision(false)
{
}
