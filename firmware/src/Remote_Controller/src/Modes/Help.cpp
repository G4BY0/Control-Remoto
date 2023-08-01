//Copyright Grupo 11, Inc. All Rights Reserved.
/***********************************************
 * * * * * * * * * * * * * * * * * * * * * * * *
 * \file
 * Source file of Modes/Help.h
 * 
 * * * * * * * * * * * * * * * * * * * * * * * *
***********************************************/
#include "Modes/Help.h"

void MODE::helpQR( const char* text , const uint8_t version ){

  xSemaphoreTake( semaphoreDisplay , portMAX_DELAY ); // Bloquear el semáforo

  //Establezco los parametros a utilizar para la muestra a la salida del display
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.display();
  
  QRCode qrcode;
  uint8_t qrcodeData[qrcode_getBufferSize(version)];
  qrcode_initText(&qrcode , qrcodeData , version , 0 , text );

  int scale = min( SCREEN_WIDTH / qrcode.size , SCREEN_HEIGHT / qrcode.size ) * 2; 

  int shiftX = ( SCREEN_WIDTH - qrcode.size * scale ) / 2;
  int shiftY = ( SCREEN_HEIGHT - qrcode.size * scale ) / 2;

  for(uint8_t y = 0 ; y < qrcode.size ; y++){
    for(uint8_t x = 0 ; x < qrcode.size ; x++){
      if(qrcode_getModule(&qrcode , x , y ))
        display.fillRect( shiftX + x * scale , shiftY + y * scale , scale , scale , SH110X_WHITE );
    }

  }
  display.display();

  //Si se presiona cualquier boton...
  while(!(buttonState(PIN::Buttons::BACK)  == HIGH  ||
          buttonState(PIN::Buttons::UP)    == HIGH  ||
          buttonState(PIN::Buttons::DOWN)  == HIGH  ||
          buttonState(PIN::Buttons::LEFT)  == HIGH  ||
          buttonState(PIN::Buttons::RIGHT) == HIGH  || 
          buttonState(PIN::Buttons::ENTER) == HIGH    ));
  delay(DEBOUNCE_TIME);  // DELAY PARA EL REBOTE DEL PULSADOR DE FENOMENO MECANICO  

  xSemaphoreGive( semaphoreDisplay ); // Desbloquear el semáforo

}