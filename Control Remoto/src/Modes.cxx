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
    case 5:     help__();                   break; //Entorno de Ayuda al usuario
    default:                                break; //IGNORE

  }

}

void MODE::profiles__(void)        { Interface::profiles();          }
void MODE::addProfile__(void)      { Interface::addProfile();        }
void MODE::deleteProfile__(void)   { Interface::deleteProfile();     }
void MODE::addSubProfile__(void)   { Interface::createSubProfile();  }
void MODE::deleteSubProfile__(void){ Interface::deleteSubProfile();  }

#pragma region Developing-Future
void MODE::help__(void)            { Interface::help(URL_USER_HELP); /*Apartado de Joaco Para el desarrollo de la pagina*/ }
#pragma endregion //Developing-Future (Opciones adicionales en un futuro)

#pragma region Shutdown
void Shutdown::displayService(void){ /*Aun no Desarrollado*/ }
void Shutdown::SDService(void)     { /*Aun no Desarrollado*/ }
void Shutdown::buttonsWaiting(void){ /*Aun no Desarrollado*/ }
#pragma endregion //Shutdown