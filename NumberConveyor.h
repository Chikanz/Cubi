// NumberConveyor.h

#ifndef _NUMBERCONVEYOR_h
#define _NUMBERCONVEYOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class NumberConveyorClass
{
 protected:


 public:
	void init();
};

extern NumberConveyorClass NumberConveyor;

#endif

