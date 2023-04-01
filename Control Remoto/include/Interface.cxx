#include "Interface.h"

/*! @brief INICIALIZACION DE BOTONES COMO ENTRADA 
    @note Arduino por defecto establece como entrada los pines digitales*/
void buttonBegin(void){

  pinMode(PIN::Buttons::UP,     INPUT);
  pinMode(PIN::Buttons::DOWN,   INPUT);
  pinMode(PIN::Buttons::LEFT ,  INPUT);
  pinMode(PIN::Buttons::RIGHT , INPUT);
  pinMode(PIN::Buttons::BACK ,  INPUT);
  pinMode(PIN::Buttons::ENTER,  INPUT);

}

inline bool buttonState(const uint8_t PIN_BUTTON) { 
  return (digitalRead(PIN_BUTTON)); 
}

Cursor::Cursor(const uint8_t AMOUNT_OF_OPTIONS__){ 
  this->AMOUNT_OF_OPTIONS = AMOUNT_OF_OPTIONS__; 
}

Cursor::Cursor(void){}


const int8_t Cursor::options(void){
  
  uint16_t moving = 0;
  while(INFINITE_LOOPING){

    if(buttonState(PIN::Buttons::DOWN) == TRUE_PULLDOWN) {
      if(moving = 1) {
        moving = AMOUNT_OF_OPTIONS; // HACE OVERFLOW DE OPCIONES
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
    else if(buttonState(PIN::Buttons::BACK) == TRUE_PULLDOWN)  { return(BUTTON_BACK_PRESSED); }

  }

}

const char* Cursor::Writer_ptr(void){
  char* pointer;
  while(pointer != '\0') {
    /* DEVELOPING

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

    */
    if(buttonState(PIN::Buttons::ENTER) == TRUE_PULLDOWN) { return ("ENTER"); }
    if(buttonState(PIN::Buttons::BACK) == TRUE_PULLDOWN) { return ("BACK"); }
  }

}

void displayBegin(void){

  display.begin(I2C_ADDRESS, true);
  display.display();
  display.clearDisplay();
  display.display();

}

const enum Interface::hub(void){

  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  
  display.setCursor(LINE_STRING_X,LINE_STRING_Y[1]);
  display.print    ("  PROFILES"   );

  display.setCursor(LINE_STRING_X,LINE_STRING_Y[2]);
  display.print    ("  ADD PROFILE");

  display.setCursor(LINE_STRING_X,LINE_STRING_Y[3]);
  display.print    ("  DELETE PROFILE");

  display.display();

  Cursor cursor(3);

  switch (cursor.options()) {
  
  case 0: // NO HACE NADA PORQUE NO SE PUEDE VOLVER PARA ATRAS EN EL HUB

  case 1: return (MODE::PROFILES); 

  case 2: return (MODE::ADDPROFILE); 

  case 3: return (MODE::DELETEPROFILE); 

  default: 

  Serial.println(SYSTEM_STRING_ERROR);
  display.setCursor(0,0);
  display.print(SYSTEM_STRING_ERROR);

  }

}

const enum Interface::profiles(void){}

const enum Interface::addProfile(void){}

const enum Interface::deleteProfile(void){}