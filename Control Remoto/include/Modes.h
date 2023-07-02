//Copyright Grupo 11, Inc. All Rights Reserved.
/***********************************************
 * * * * * * * * * * * * * * * * * * * * * * * *
 * \file
 * Manejo de modos para el usuario
 *  - Interfaz De Usuario ----> (EN DESARROLLO) MODO "HELP"
 *  - (EN DESARROLLO) Modo "Sleeping"
 * * * * * * * * * * * * * * * * * * * * * * * *
***********************************************/
#ifndef MODES_H
#define MODES_H

#define URL_USER_HELP "https://drive.google.com/drive/folders/1oE-sRMhUQJEPQ91XLznxZHdQJ9x4quRs?usp=drive_link"

#include "Interface.h"

namespace MODE{

    /*! @brief Seleccion de Modos (Hub, Principal) */
    void hub__(void);

    /*! @brief Seleccion de Perfiles */
    void profiles__(void);

    /*! @brief Agregar Perfil */
    void addProfile__(void);

    /*! @brief Eliminar Perfil */
    void deleteProfile__(void);

    /*! @brief Agregar Subperfil a un Perfil (Pide a el Usuario el perfil a agregar) */
    void addSubProfile__(void);
    
    /*! @brief Eliminar Subperfil a un Perfil (Pide a el Usuario el perfil a eliminar) */
    void deleteSubProfile__(void);

    #pragma region Developing-Future
    /*! @brief Muestra apartado de funcionamiento del dispositivo*/
    void help__(void);
    #pragma endregion
    
};

#pragma region Shutdown
namespace Shutdown{

    /*! @brief Stoppea el servicio visual
        @note Aun no Desarrollado */
    void displayService(void);

    /*! @brief Stoppea el servicio de almacenamiento
        @note Aun no Desarrollado */
    void SDService(void);

    /*! @brief Prepara los botones en espera hasta alguna respuesta (presionando rompe el loop)
        @note Aun no Desarrollado */
    void buttonsWaiting(void);

};
#pragma endregion //Shutdown

#endif