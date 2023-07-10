//Copyright Grupo 11, Inc. All Rights Reserved.
/***********************************************
 * * * * * * * * * * * * * * * * * * * * * * * *
 * \file
 * Source code of members for Modes.h
 * 
 * * * * * * * * * * * * * * * * * * * * * * * *
***********************************************/
#include "Modes.h"

void MODE::hub(void){

  switch ( Interface::hub() ){

    case 0:     profiles();               break; //Manejo de Perfiles y Subperfiles Almacenados
    case 1:     addProfile();             break; //Agregar un Perfil del almacenamiento 
    case 2:     deleteProfile();          break; //Eliminar un Perfil del almacenamiento
    
    #pragma region Pre-Alpha Modes
    case 3:     addSubProfile();          break; //Agregar Subperfil de un Perfil dado
    case 4:     deleteSubProfile();       break; //Eliminar Subperfil de un Perfil dado
    #pragma endregion
    case 5:     help();                   break; //Entorno de Ayuda al usuario
    case 6:     ShutDown::now();            break; //Modo Apagado (Deja en espera los botones en caso de querer volver a iniciar)
    default:                                break; //IGNORE

  }

}

void MODE::profiles(void)           { Interface::profiles();          }
void MODE::addProfile(void)         { Interface::addProfile();        }
void MODE::deleteProfile(void)      { Interface::deleteProfile();     }
void MODE::addSubProfile(void)      { Interface::createSubProfile(nullptr);  }
void MODE::deleteSubProfile(void)   { Interface::deleteSubProfile(nullptr);  }

#define QRVERSION 4
#define URL_USER_HELP "https://drive.google.com/file/d/1hc0Sb17FSV1dKWV1zGJdsFk9_PXR-PD6/view"
void MODE::help(void)               { Interface::help(URL_USER_HELP , QRVERSION ); /*Apartado de Joaco Para el desarrollo de la pagina*/ }


void MODE::sleep(uint32_t Seconds){
  auto countSleep = millis();
  MODE::ShutDown::buttonsWaiting();
  yield();
  if( ( millis() - countSleep ) >= Seconds ) MODE::ShutDown::now();
}

volatile bool MODE::ShutDown::shouldRestart = false;

void MODE::ShutDown::now(void) { 
  //Servicio de pantalla off
  displayService(); 
  //Servicio de Almacenamiento off
  SDService();
  //Servicio de comunicacion SPI off
  SPI.end(); 

  //Aviso que se entro en el modo ShutDown
  Serial.println(F("Entrando al modo ShutDown."));
  // Asegura que se envíen los datos pendientes antes de apagar el Serial
  Serial.flush(); 
  //Servicio de comunicacion Serial (Rx/Tx) off
  Serial.end(); 

  //Servicio de botonera en espera
  buttonsWaiting();
  
  yield(); // Realimento el Watch Dog por si se produce larga la espera
  //Finalizo todos los Task menos este...

  TaskStatus_t taskStatus;
  
  // Iterar a través de todos los tasks
  /* while (uxTaskGetSystemState(&taskStatus, configMAX_TASK_NAME_LEN, NULL) > 1) { // 1 porque Considerando que solo esté este mismo. // NO FUNCIONA uxTaskGetSystemState() undefined reference to `uxTaskGetSystemState'
  
    //TaskHandle_t taskHandle = taskStatus.xHandle;

    if(sleepIndicateTasks == xTaskGetCurrentTaskHandle())
    continue;

    // Eliminar el task
    vTaskDelete(sleepIndicateTasks);
  }*/

  shouldRestart = true;

  // Código de limpieza y finalización del task
  vTaskDelete(NULL); // Delete este mismo Task

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
