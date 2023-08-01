//Copyright Grupo 11, Inc. All Rights Reserved.
/***********************************************
 * * * * * * * * * * * * * * * * * * * * * * * *
 * 
 * Clock Mode which it's shown in the lobby
 *
 * * * * * * * * * * * * * * * * * * * * * * * *
***********************************************/
#ifndef CLOCK_H
#define CLOCK_H

// DIGITAL CLOCK TO DISPLAY
#define CLOCK_DIGITAL_STATUS __clock_digital
extern bool __clock_digital; // If it is '1'. its enabled, else ('0') disabled

// ANALOG CLOCK TO DISPLAY
#define CLOCK_ANALOG_STATUS __clock_analog
extern bool __clock_analog; // If it is '1'. its enabled, else ('0') disabled



#endif