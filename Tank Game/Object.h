#pragma once
#include <allegro5/allegro.h>

class Object
{
 public:
    Object();
    virtual ~Object();

 private: 
     int xpos, ypos, width, height;
     ALLEGRO_BITMAP** sprite;
};

