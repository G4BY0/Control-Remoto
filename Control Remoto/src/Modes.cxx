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
void MODE::Sleep::now(void)           { 
  Sleep::displayService(); Sleep::SDService(); Sleep::buttonsWaiting(); 
  //Inicializacion del sistema del display
  displayBegin();     Serial.println(F("Display Inicializado"));
  
  //Inicializacion del sistema de botones
  buttonsBegin();     Serial.println(F("Botonera Inicializada"));
  
  //Inicializacion del sistema de almacenamiento
  SDBegin();          Serial.println(F("Almacenamiento Inicializado"));
  
  //Inicializacion del sistema del infrarrojo
  infraredBegin();    Serial.println(F("Infrared Inicializado"));
  //Espero a que todos los procesos terminen para inicializar
  yield();
}
void MODE::Sleep::displayService(void){ display.clearDisplay(); display.display(); }
void MODE::Sleep::SDService(void)     { SD.end(); }
void MODE::Sleep::buttonsWaiting(void){ 
  //Logica de si se llegara a pulsar cualquier boton
  while(!(  buttonState(PIN::Buttons::BACK ) == HIGH  ||
            buttonState(PIN::Buttons::UP   ) == HIGH  ||
            buttonState(PIN::Buttons::DOWN ) == HIGH  ||
            buttonState(PIN::Buttons::LEFT ) == HIGH  ||
            buttonState(PIN::Buttons::RIGHT) == HIGH  || 
            buttonState(PIN::Buttons::ENTER) == HIGH     ));
}
#pragma endregion //Sleep