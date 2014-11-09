#pragma once

#include <iostream>
#include <string>
#include <sstream>
using namespace std;

#define LOW_DEBUG 1         // Points of the code that only run one
#define MED_DEBUG 1<<1      // Points of the code that are run under a certain user 
#define HI_DEBUG  1<<2      // Points of the code that are independed of user input
#define GRAPHIC_DEBUG  1<<3 // graphical debugs

#define DEBUG (LOW_DEBUG | MED_DEBUG | HI_DEBUG) // set what debugging message to show here

#define PRINT_DEBUG(x, type) if(DEBUG & type) x;
