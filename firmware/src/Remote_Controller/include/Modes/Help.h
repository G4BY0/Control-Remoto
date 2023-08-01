//Copyright Grupo 11, Inc. All Rights Reserved.
/***********************************************
 * * * * * * * * * * * * * * * * * * * * * * * *
 * \file
 * Help for users
 * 
 * * * * * * * * * * * * * * * * * * * * * * * *
***********************************************/

#ifndef HELP_H
#define HELP_H

#include <cstdint>
#include <freertos/FreeRTOS.h>  //FreeRTOS Operative System
#include <freeRTOS/task.h>      //MultiThreading
#include <freeRTOS/semphr.h>    //Semaphore
#include <QRRCODE.h>
#include "System.h"
#include "Modes/Modes.h"
#include "Interface.h"


namespace MODE{
    /*! \brief    Muestra las instrucciones de como se usa el dispositivo a traves de un QR
        \param    text Recibe String que se codificara en el codigo QR
        \param    version Version del QR (permite mas o menos informacion dentro)
        \note     El usuario debe scanear el QR generado y recibirá el string del parametro */
    void helpQR(const char* text , const uint8_t version = 4);

};

#endif