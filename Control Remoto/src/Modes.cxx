//Copyright Grupo 11, Inc. All Rights Reserved.
/***********************************************
 * * * * * * * * * * * * * * * * * * * * * * * *
 * \file
 * Source code of members for Modes.h
 * 
 * * * * * * * * * * * * * * * * * * * * * * * *
***********************************************/
#include "Modes.h"

void MODE::hub__(void){

  switch (Interface::hub()){

    case 0:     profiles__();               break; //Manejo de Perfiles y Subperfiles Almacenados
    case 1:     addProfile__();             break; //Agregar un Perfil del almacenamiento 
    case 2:     deleteProfile__();          break; //Eliminar un Perfil del almacenamiento
    
    #pragma region Pre-Alpha Modes
    case 3:     addSubProfile__();          break; //Agregar Subperfil de un Perfil dado
    case 4:     deleteSubProfile__();       break; //Eliminar Subperfil de un Perfil dado
    #pragma endregion
    case 5: help__();                       break; //Entorno de Ayuda al usuario
    default: exit(1);                       break; //Sale del void loop() .Finaliza el sistema

  }

}

inline void MODE::profiles__(void)        { Interface::profiles();          }
inline void MODE::addProfile__(void)      { Interface::addProfile();        }
inline void MODE::deleteProfile__(void)   { Interface::deleteProfile();     }
inline void MODE::addSubProfile__(void)   { Interface::createSubProfile();  }
inline void MODE::deleteSubProfile__(void){ Interface::deleteSubProfile();  }

#pragma region Developing-Future
inline void MODE::help__(void)            { Interface::help(); /*Apartado de Joaco Para el desarrollo de la pagina*/ }
#pragma endregion //Developing-Future (Opciones adicionales en un futuro)

#pragma region Shutdown
inline void Shutdown::displayService(void){ /*Aun no Desarrollado*/ }
inline void Shutdown::SDService(void)     { /*Aun no Desarrollado*/ }
inline void Shutdown::buttonsWaiting(void){ /*Aun no Desarrollado*/ }
#pragma endregion //Shutdown