
#include "Modes/Clock.h"

#define CLOCK_DIGITAL_STATUS __clock_digital
bool __clock_digital = 1; //By default it is enabled (Is replaced by config.ini)

#define CLOCK_ANALOG_STATUS __clock_analog
bool __clock_analog = 1; //By default it is enabled (Is replaced by config.ini)