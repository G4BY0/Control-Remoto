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
    display.printf( "Clock: %s\n"     ,__clock      ? "ON" : "OFF"  ); 
    display.printf( "Wi-Fi: %s\n"     ,__wifi       ? "ON" : "OFF"  ); 
    display.printf( "Bluetooth: %s\n" ,__bluetooth  ? "ON" : "OFF"  );  
  
    // Memoria disponible aun
    display.printf("Free storage: %llu\n" ,[&] () -> uint64_t{
      return (SD.cardSize() - SD.usedBytes() == 0ull) ? 0ull : ( std::log10(SD.cardSize() - SD.usedBytes()) + 1ull );
    }());
    
    //Velocidad de CPU
    display.printf("CPU speed: %lu\n" ,ESP.getCpuFreqMHz() );
  
    display.display();
  
    xSemaphoreGive( semaphoreDisplay ); // Desbloquear el semáforo
  
  } delay(DEBOUNCE_TIME);  // DELAY PARA EL REBOTE DEL PULSADOR DE FENOMENO MECANICO  

}