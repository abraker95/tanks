#include "Object.h"

Object::Object()
{

}

Object::~Object()
{
 al_destroy_bitmap(*sprite);
}
