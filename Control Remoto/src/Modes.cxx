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
    case 6:     Sleep::now();               break;
    default:                                break; //IGNORE

  }

}

void MODE::profiles__(void)           { Interface::profiles();          }
void MODE::addProfile__(void)         { Interface::addProfile();        }
void MODE::deleteProfile__(void)      { Interface::deleteProfile();     }
void MODE::addSubProfile__(void)      { Interface::createSubProfile();  }
void MODE::deleteSubProfile__(void)   { Interface::deleteSubProfile();  }

#define URL_USER_HELP "https://drive.google.com/file/d/1hc0Sb17FSV1dKWV1zGJdsFk9_PXR-PD6/view"
void MODE::help__(void)               { Interface::help(URL_USER_HELP); /*Apartado de Joaco Para el desarrollo de la pagina*/ }

#pragma region Sleep
void MODE::Sleep::now(void) { 
  //Servicio de pantalla off
  //Sleep::displayService();
  //Servicio de Almacenamiento off
  //Sleep::SDService();
  //Servicio de comunicacion Serial (Rx/Tx) off
  // ---->>>>>>>>>>>>>>> Serial.end(); dangerous relocation: windowed longcall crosses 1GB boundary; return may fail: *UND*
  //Servicio de comunicacion SPI off
  // ---->>>>>>>>>>>>>>> SPI.end(); dangerous relocation: windowed longcall crosses 1GB boundary; return may fail: *UND*
  //Servicio de botonera en espera
  Sleep::buttonsWaiting();

  //Reinicio del Sistema, se despierta del sleeping
  // Descomentar en caso de querer usar esta opcion, Salta a la direccion 0 de memoria (inicio del programa) pero no vuelve a incializar los puertos
  //asm volatile("jmp 0x00"); 
  // Descomentar en caso de querer usar esta opcion, Se redirige a la direccion 0 de memoria (inicio del programa) pero si puede generar comportamientos indefinidos
  Sleep::resetNOW();
  //En el caso de volver a querer inicializar los puertos se deberia utilizar el timer watch dog pero al usarla estaria inabilitandolo para usarlo en este caso exclusivo
    //Aun no desarrollada esta alternativa
}
void MODE::Sleep::displayService(void){ display.clearDisplay(); display.display(); }
void MODE::Sleep::SDService(void)     { /*SD.end();*/ }
void MODE::Sleep::buttonsWaiting(void){ 
  //Logica de si se llegara a pulsar cualquier boton
  /*while(!(  buttonState(PIN::Buttons::BACK ) == HIGH  ||
            buttonState(PIN::Buttons::UP   ) == HIGH  ||
            buttonState(PIN::Buttons::DOWN ) == HIGH  ||
            buttonState(PIN::Buttons::LEFT ) == HIGH  ||
            buttonState(PIN::Buttons::RIGHT) == HIGH  || 
            buttonState(PIN::Buttons::ENTER) == HIGH     ));*/
}
#pragma endregion //Sleep