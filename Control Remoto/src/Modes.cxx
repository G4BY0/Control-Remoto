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
    case 6:     ShutDown::now();            break; //Modo Apagado (Deja en espera los botones en caso de querer volver a iniciar)
    default:                                break; //IGNORE

  }

}

void MODE::profiles__(void)           { Interface::profiles();          }
void MODE::addProfile__(void)         { Interface::addProfile();        }
void MODE::deleteProfile__(void)      { Interface::deleteProfile();     }
void MODE::addSubProfile__(void)      { Interface::createSubProfile(nullptr);  }
void MODE::deleteSubProfile__(void)   { Interface::deleteSubProfile(nullptr);  }

#define URL_USER_HELP "https://drive.google.com/file/d/1hc0Sb17FSV1dKWV1zGJdsFk9_PXR-PD6/view"
void MODE::help__(void)               { Interface::help(URL_USER_HELP , 4); /*Apartado de Joaco Para el desarrollo de la pagina*/ }


void MODE::ShutDown::now(void) { 
  //Servicio de pantalla off
  displayService();
  //Servicio de Almacenamiento off
  SDService();
  //Servicio de comunicacion SPI off
  SPI.end(); 

  //Aviso que se entro en el modo ShutDown
  Serial.println(F("Entrando al modo ShutDown."));
  //Servicio de comunicacion Serial (Rx/Tx) off
  Serial.end(); 

  #if defined(__AVR__)
  // Reducir la velocidad del reloj dividido 8
  clock_prescale_set(clock_div_8);
  Serial.println(F("Velocidad del reloj reducida a 80 MHz."));
  #endif

  //Servicio de botonera en espera
  buttonsWaiting();

  #if defined(__AVR__)
  // Restaurar la velocidad del reloj a la configuración predeterminada
  clock_prescale_set(clock_div_1);
  Serial.println(F("Velocidad del reloj devuelta a la normalidad."));
  #endif

  //Reinicio del Sistema, se despierta del sleeping
  #if defined(ESP32) || defined(ESP8266)
  ESP.restart();
  #else
  asm volatile("reset"); 
  #endif
}

void MODE::ShutDown::displayService(void){ display.clearDisplay(); display.display(); }

void MODE::ShutDown::SDService(void)     { SD.end(); }

void MODE::ShutDown::buttonsWaiting(void){ 

  //Logica de si se llegara a pulsar cualquier boton
  while(!(  buttonState(PIN::Buttons::BACK ) == HIGH  ||
            buttonState(PIN::Buttons::UP   ) == HIGH  ||
            buttonState(PIN::Buttons::DOWN ) == HIGH  ||
            buttonState(PIN::Buttons::LEFT ) == HIGH  ||
            buttonState(PIN::Buttons::RIGHT) == HIGH  || 
            buttonState(PIN::Buttons::ENTER) == HIGH     ));

}
