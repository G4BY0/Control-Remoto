//Copyright Grupo 11, Inc. All Rights Reserved.
/***********************************************
 * * * * * * * * * * * * * * * * * * * * * * * *
 * \file
 * Manejo de modos para el usuario
 * 
 * * * * * * * * * * * * * * * * * * * * * * * *
***********************************************/

#ifndef MODES_HPP
#define MODES_HPP

#include "Interface.h"

namespace MODE{

    /*! @brief Seleccion de Modos (Hub, Principal)*/
    void hub__(void);

    /*! @brief Seleccion de Perfiles*/
    void profiles__(void);

    /*! @brief Agregar Perfil*/
    void addProfile__(void);

    /*! @brief Eliminar Perfil*/
    void deleteProfile__(void);

    /*! @brief Agregar Subperfil a un Perfil*/
    void addSubProfile__(void);
    
    /*! @brief Eliminar Subperfil a un Perfil*/
    void deleteSubProfile__(void);

    #pragma region Developing-Future
    /*! @brief Muestra apartado de funcionamiento del dispositivo*/
    void help__(void);
    #pragma endregion
    
};

#pragma region Shutdown
namespace Shutdown{

    /*! @brief Stoppea el servicio visual*/
    void displayService(void);

    /*! @brief Stoppea el servicio de almacenamiento*/
    void SDService(void);

    /*! @brief Prepara los botones en espera hasta alguna respuesta (presionando)*/
    void buttonsWaiting(void);

};
#pragma endregion //Shutdown

#endif