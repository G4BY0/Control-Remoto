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

    /*! @brief Seleccion de Modos (Hub, Principal) */
    inline void hub__(void);

    /*! @brief Seleccion de Perfiles */
    inline void profiles__(void);

    /*! @brief Agregar Perfil */
    inline void addProfile__(void);

    /*! @brief Eliminar Perfil */
    inline void deleteProfile__(void);

    /*! @brief Agregar Subperfil a un Perfil (Pide a el Usuario el perfil a agregar) */
    inline void addSubProfile__(void);
    
    /*! @brief Eliminar Subperfil a un Perfil (Pide a el Usuario el perfil a eliminar) */
    inline void deleteSubProfile__(void);

    #pragma region Developing-Future
    /*! @brief Muestra apartado de funcionamiento del dispositivo*/
    inline void help__(void);
    #pragma endregion
    
};

#pragma region Shutdown
namespace Shutdown{

    /*! @brief Stoppea el servicio visual*/
    inline void displayService(void);

    /*! @brief Stoppea el servicio de almacenamiento*/
    inline void SDService(void);

    /*! @brief Prepara los botones en espera hasta alguna respuesta (presionando)*/
    inline void buttonsWaiting(void);

};
#pragma endregion //Shutdown

#endif