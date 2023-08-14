//Copyright Grupo 11, Inc. All Rights Reserved.
/***********************************************
 * * * * * * * * * * * * * * * * * * * * * * * *
 * 
 * Source of Diagnostics.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * *
***********************************************/

#include "Modes/Diagnostics.h"

void MODE::showDiagnostics(void){

  //Si se presiona cualquier boton... FIN DEL MENU: "Diagnostics"
  while(!(buttonState(PIN::Buttons::BACK)  == HIGH  ||
          buttonState(PIN::Buttons::UP)    == HIGH  ||
          buttonState(PIN::Buttons::DOWN)  == HIGH  ||
          buttonState(PIN::Buttons::LEFT)  == HIGH  ||
          buttonState(PIN::Buttons::RIGHT) == HIGH  || 
          buttonState(PIN::Buttons::ENTER) == HIGH    )){

  xSemaphoreTake( semaphoreDisplay , portMAX_DELAY ); // Bloquear el semáforo

    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);
    display.clearDisplay();
    display.setCursor( 0 , 0 );
  
    // Servicios de Clock, Wifi, Bluetooth
    display.println(F("Services:"));
    display.printf( "Clock: %s\n"     ,__clock      ? "true" : "false"  ); 
    display.printf( "Wi-Fi: %s\n"     ,__wifi       ? "true" : "false"  ); 
    display.printf( "Bluetooth: %s\n" ,__bluetooth  ? "true" : "false"  );  
  
    // Memoria disponible aun
    display.printf("Free storage: %llu\n" ,[freespace = SD.cardSize() - SD.usedBytes()] () -> uint64_t{
      return (freespace == 0ull) ? 0ull : ( std::log10(freespace) + 1ull );
    }());
    
    //Velocidad de CPU
    display.printf("CPU speed: %lu\n" ,ESP.getCpuFreqMHz() );
  
    display.display();
  
    xSemaphoreGive( semaphoreDisplay ); // Desbloquear el semáforo
  
  } delay(DEBOUNCE_TIME);  // DELAY PARA EL REBOTE DEL PULSADOR DE FENOMENO MECANICO  

}