//Copyright Grupo 11, Inc. All Rights Reserved.
/***********************************************
 * * * * * * * * * * * * * * * * * * * * * * * *
 * \file
 * Manejo de modos para el usuario
 * 
 * * * * * * * * * * * * * * * * * * * * * * * *
***********************************************/

#ifndef MODES_H
#define MODES_H

#include "Interface.h"

namespace MODE{

    void hub__(void);
    inline void profiles__(void);
    inline void addProfile__(void);
    inline void deleteProfile__(void);
    inline void addSubProfile__(void);
    inline void deleteSubProfile__(void);

    #pragma region Shutdown
    namespace Shutdown{

    void displayService(void);
    void SDService(void);
    void buttonsWaiting(void);

    }
    #pragma endregion //Shutdown
};

#endif