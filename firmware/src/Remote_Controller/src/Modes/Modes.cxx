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
    case 0:     profiles();        break; //Manejo de Perfiles y Subperfiles Almacenados
    case 1:     edit();            break;
    case 2:     help();            break; // Modo Entorno de Ayuda al usuario
    case 3:     ShutDown_now();    break; // Modo Apagado
    case 4:     showDiagnostics(); break; // Modo Diagnosticos
  }
}

void MODE::profiles(void)           { Interface::profiles();          }
void MODE::edit(void)               { Interface::edit(); }

void MODE::help(void)               { helpQR(URL_USER_HELP , QRVERSION ); /*Apartado de Joaco Para el desarrollo de la pagina*/ }
#undef QRVERSION
#undef URL_USER_HELP