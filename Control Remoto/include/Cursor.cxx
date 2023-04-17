#include "Cursor.h"

Cursor::Cursor(const uint8_t AMOUNT_OF_OPTIONS__){ 
  this->AMOUNT_OF_OPTIONS = AMOUNT_OF_OPTIONS__; 
}

Cursor::Cursor(void){


}


const uint8_t Cursor::options(void){
  
  uint8_t moving = 0;
  while(INFINITE_LOOPING){

    if(buttonState(PIN::Buttons::DOWN) == TRUE_PULLDOWN) {
      if(moving = AMOUNT_OF_OPTIONS) {
        moving = 1; // HACE OVERFLOW DE OPCIONES
        display.setCursor(LINE_CURSOR_X, LINE_STRING_Y[3]); // POSICION DEL CURSOR UN INSTANTE ANTES
        display.print(" "); // ESCRIBO UN ESPACIO PARA BORRAR EL CURSOR QUE ESTUVO UN INSTANTE ANTES

        display.setCursor(LINE_CURSOR_X, LINE_STRING_Y[moving]); // POSICION DEL CURSOR
        display.print(CURSOR_CHARACTER); // DIBUJO EN PANTALLA EL CURSOR
        display.display(); // EXPULSO A RAM LAS INSTRUCCIONES DEL DISPLAY
      }
      else {
        moving++;
        
        display.setCursor(LINE_CURSOR_X, LINE_STRING_Y[moving-1]); // POSICION DEL CURSOR UN INSTANTE ANTES
        display.print(" "); // ESCRIBO UN ESPACIO PARA BORRAR EL CURSOR QUE ESTUVO UN INSTANTE ANTES

        display.setCursor(LINE_CURSOR_X, LINE_STRING_Y[moving]); // POSICION DEL CURSOR
        display.print(CURSOR_CHARACTER); // DIBUJO EN PANTALLA EL CURSOR
        display.display(); // EXPULSO A RAM LAS INSTRUCCIONES DEL DISPLAY
      }
    }

    else if(buttonState(PIN::Buttons::UP) == TRUE_PULLDOWN) {
      if(moving = AMOUNT_OF_OPTIONS) {
        moving = 1; // HACE OVERFLOW DE OPCIONES
      }
      else {
        moving--;
        display.setCursor(LINE_CURSOR_X, LINE_STRING_Y[moving+1]); // POSICION DEL CURSOR UN INSTANTE ANTES
        display.print(" "); // ESCRIBO UN ESPACIO PARA BORRAR EL CURSOR QUE ESTUVO UN INSTANTE ANTES

        display.setCursor(LINE_CURSOR_X, LINE_STRING_Y[moving]); // POSICION DEL CURSOR
        display.print(CURSOR_CHARACTER); // DIBUJO EN PANTALLA EL CURSOR
        display.display(); // EXPULSO A RAM LAS INSTRUCCIONES DEL DISPLAY
      }
    }

    else if(buttonState(PIN::Buttons::ENTER) == TRUE_PULLDOWN) { return(moving);              }
    else if(buttonState(PIN::Buttons::BACK) == TRUE_PULLDOWN)  { return(BUTTON_PRESSED_BACK); }

  }

}

const char* Cursor::writer_ptr(void){
  /*
  char* pointer;
  while(pointer != '\0') {
     DEVELOPING

    if(buttonState(PIN::Buttons::UP) == TRUE_PULLDOWN) {
      if(moving = AMOUNT_OF_OPTIONS) {
        moving = 1; // HACE OVERFLOW DE OPCIONES
      }
      else {
        moving--;
        display.setCursor(,); // POSICION DEL CURSOR UN INSTANTE ANTES
        display.print(" "); // ESCRIBO UN ESPACIO PARA BORRAR EL CURSOR QUE ESTUVO UN INSTANTE ANTES

        display.setCursor(LINE_CURSOR_X, LINE_STRING_Y[moving]); // POSICION DEL CURSOR
        display.print(CURSOR_CHARACTER); // DIBUJO EN PANTALLA EL CURSOR
        display.display(); // EXPULSO A RAM LAS INSTRUCCIONES DEL DISPLAY
      }
    }

    
    if(buttonState(PIN::Buttons::ENTER) == TRUE_PULLDOWN) { return ("ENTER"); }
    if(buttonState(PIN::Buttons::BACK) == TRUE_PULLDOWN) { return ("BACK"); }
    
  }  
  */  

}