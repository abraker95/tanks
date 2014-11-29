#pragma once

#include <iostream>
#include <assert.h>
using namespace std;

#define OFF_DEBUG	   0<<0 // Turn off the debug
#define LOW_DEBUG      1<<0 // Parts of the code that only run one
#define MED_DEBUG      1<<1 // Parts of the code that are run under a certain user 
#define HI_DEBUG	   1<<2 // Parts of the code that are independed of user input
#define GRAPHIC_DEBUG  1<<3 // graphical debugs

#define ISYS	1<<4 // Input system
#define PHYSYS	1<<5 // Physics system
#define GFXSYS	1<<6 // Graphics System
#define TXTRSYS 1<<7 // Texture system
#define EXPSYS	1<<8 // Expire System
#define ENVSYS  1<<9 // Environment and Application
#define MASTERSYS (ISYS | PHYSYS | GFXSYS | TXTRSYS | EXPSYS) // All subsystems

#define SYSTEM (ISYS | PHYSYS | GFXSYS | TXTRSYS | EXPSYS | ENVSYS)   // set what system it is part off here
#define DEBUG (LOW_DEBUG | MED_DEBUG | HI_DEBUG)			          // set what type of debugging message to show here

#define PRINT_DEBUG(x, type, sys) if((DEBUG & type) && (SYSTEM & sys)) x;
