//Copyright Grupo 11, Inc. All Rights Reserved.
/***********************************************
 * * * * * * * * * * * * * * * * * * * * * * * *
 * \file
 * Manejo de modos para el usuario
 *  - Interfaz De Usuario 
 * * * * * * * * * * * * * * * * * * * * * * * *
***********************************************/
#ifndef MODES_H
#define MODES_H

#include "System.h"
#include "Interface.h"
#include "Diagnostics.h"
#include "Help.h"
#include <esp_sleep.h>
#include "System.h"

/*! \brief Modos del Control Remoto*/
namespace MODE{

    /*! \brief Seleccion de Modos (Hub, Principal) */
    void hub(void);

    /*! \brief Seleccion de Perfiles */
    void profiles(void);

    void edit(void);

    #define QRVERSION 4 // Version del QR
    #define URL_USER_HELP "https://drive.google.com/file/d/1hc0Sb17FSV1dKWV1zGJdsFk9_PXR-PD6/view" // String a codificar
    /*! \brief Muestra apartado de funcionamiento del dispositivo*/
    void help(void);

};

#endif // Modes_h