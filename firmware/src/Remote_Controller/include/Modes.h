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

#include "Tasks.h"
#include "Interface.h"

#if defined(__AVR__)
    #include <avr/power.h>
#endif

#if defined(ESP32) || defined(ESP8266)
    #include <esp_sleep.h>
#endif

/*! \brief Modos del Control Remoto*/
namespace MODE{

    /*! \brief Seleccion de Modos (Hub, Principal) */
    void hub(void);

    /*! \brief Seleccion de Perfiles */
    void profiles(void);

    /*! \brief Agregar Perfil */
    void addProfile(void);

    /*! \brief Eliminar Perfil */
    void deleteProfile(void);

    /*! \brief Agregar Subperfil a un Perfil (Pide a el Usuario el perfil a agregar) */
    void addSubProfile(void);
    
    /*! \brief Eliminar Subperfil a un Perfil (Pide a el Usuario el perfil a eliminar) */
    void deleteSubProfile(void);

    /*! \brief Muestra apartado de funcionamiento del dispositivo*/
    void help(void);
    
    /*! \brief Deja el dispositivo en espera a respuesta del usuario.
        \param Seconds si pasa el tiempo estipulado, se coloca el dispositivo en MODE::ShutDown */
    void sleep(uint32_t Seconds);

    /*! \brief Manejo de Servicios poniendo en apagado*/
    namespace ShutDown{

        // Bandera para indicar el reinicio
        extern volatile bool shouldRestart;

        /*! \brief Stoppea todos los servicios y pone en modo sleeping a la placa de desarrollo */
        void now(void);

        /*! \brief Detiene el servicio visual */
        void displayService(void);

        /*! \brief Detiene el servicio de almacenamiento */
        void SDService(void);

        /*! \brief Pone en espera los botones hasta alguna respuesta */
        void buttonsWaiting(void);
    
    };

};

#endif // Modes_h