//Copyright Grupo 11, Inc. All Rights Reserved.
/***********************************************
 * * * * * * * * * * * * * * * * * * * * * * * *
 * 
 * Shows Diagnostics of uses.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * *
***********************************************/
#ifndef DIAGNOSTICS_H
#define DIAGNOSTICS_H

#include <cmath>
#include <Adafruit_SSD1306.h>
#include <esp_system.h>
#include <pgmspace.h> // Biblioteca para acceder a datos almacenados en PROGMEM 

#include "Modes/Modes.h"
#include "System.h"

namespace MODE{
/*! \brief */
void showDiagnostics(void);
};

#endif // DIAGNOSTICS_H