//Copyright Grupo 11, Inc. All Rights Reserved.
/***********************************************
 * * * * * * * * * * * * * * * * * * * * * * * *
 * \file
 * Source code of members for Modes.h
 * 
 * * * * * * * * * * * * * * * * * * * * * * * *
***********************************************/
#include "Modes/Modes.h"

void MODE::hub(void){

  switch ( Interface::hub() ){

    case 0:     profiles();               break; //Manejo de Perfiles y Subperfiles Almacenados
    case 1:     addProfile();             break; //Agregar un Perfil del almacenamiento 
    case 2:     deleteProfile();          break; //Eliminar un Perfil del almacenamiento
    
    #pragma region Test-Here
    case 3:     addSubProfile();          break; //Agregar Subperfil de un Perfil dado
    case 4:     deleteSubProfile();       break; //Eliminar Subperfil de un Perfil dado
    #pragma endregion // Test-Here
    case 5:     help();                   break; //Entorno de Ayuda al usuario
    case 6:     ShutDown_now();          break; //Modo Apagado
    case 7:     showDiagnostics();        break; //Modo que muestra la actividad de los servicios
    


    default:                              break; //IGNORE

  }

}

void MODE::profiles(void)           { Interface::profiles();          }
void MODE::addProfile(void)         { Interface::addProfile();        }
void MODE::deleteProfile(void)      { Interface::deleteProfile();     }
void MODE::addSubProfile(void)      { Interface::createSubProfile();  }
void MODE::deleteSubProfile(void)   { Interface::deleteSubProfile();  }

void MODE::help(void)               { helpQR(URL_USER_HELP , QRVERSION ); /*Apartado de Joaco Para el desarrollo de la pagina*/ }
#undef QRVERSION
#undef URL_USER_HELP