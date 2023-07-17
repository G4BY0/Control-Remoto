//Copyright Grupo 11, Inc. All Rights Reserved.
/***********************************************
 * * * * * * * * * * * * * * * * * * * * * * * *
 * 
 * Source of Diagnostics.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * *
***********************************************/

#include "Diagnostics.h"

#define TRUE_STR 'true'
#define FALSE_STR 'false'
void getDiagnostics(void){

  // Bloquear el semáforo
  xSemaphoreTake( semaphoreDisplay , portMAX_DELAY );
  display.setCursor( 0 , 0 );
  display.setCursor( 10 , 10 );
  display.printf( 
    "CLOCK: %s\nWIFI: %s\nBLUETOOTH: %s\n" 
    ,__clock      ? TRUE_STR : FALSE_STR
    ,__wifi       ? TRUE_STR : FALSE_STR
    ,__bluetooth  ? TRUE_STR : FALSE_STR 
  );

  // Desbloquear el semáforo
  xSemaphoreGive( semaphoreDisplay );

  //Si se presiona cualquier boton...
  while(!(buttonState(PIN::Buttons::BACK)  == HIGH  ||
          buttonState(PIN::Buttons::UP)    == HIGH  ||
          buttonState(PIN::Buttons::DOWN)  == HIGH  ||
          buttonState(PIN::Buttons::LEFT)  == HIGH  ||
          buttonState(PIN::Buttons::RIGHT) == HIGH  || 
          buttonState(PIN::Buttons::ENTER) == HIGH    ));
  delay(DEBOUNCE_TIME);  // DELAY PARA EL REBOTE DEL PULSADOR DE FENOMENO MECANICO  


}