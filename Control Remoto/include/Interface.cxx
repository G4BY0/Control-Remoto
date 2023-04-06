#include "Interface.h"


void buttonsBegin(void){

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
    else if(buttonState(PIN::Buttons::BACK) == TRUE_PULLDOWN)  { return(BUTTON_PRESSED::BACK); }

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

void displayBegin(void){

  display.begin(I2C_ADDRESS, true);
  display.display();
  display.clearDisplay();
  display.display();

}




const enum MODE_t Interface::hub(void){

  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  
  display.setCursor(LINE_STRING_X,LINE_STRING_Y[0]);
  display.print    ("  PROFILES"   );

  display.setCursor(LINE_STRING_X,LINE_STRING_Y[1]);
  display.print    ("  ADD PROFILE");

  display.setCursor(LINE_STRING_X,LINE_STRING_Y[2]);
  display.print    ("  DELETE PROFILE");

  display.display();

  Cursor cursor(3);

  switch (cursor.options()) {
  
  case 0: // NO HACE NADA PORQUE NO SE PUEDE VOLVER PARA ATRAS EN EL HUB

  case 1: return (MODE_t::PROFILES); 
          break;
  case 2: return (MODE_t::ADDPROFILE); 
          break;
  case 3: return (MODE_t::DELETEPROFILE); 
          break;
  default: 

  Serial.println(SYSTEM_STRING_ERROR);
  display.setCursor(0,0);
  display.print(SYSTEM_STRING_ERROR);

  }

}

const enum MODE_t Interface::profiles(void){
  
  Profiles::showProfiles_();  // ESCRIBE EN EL SERIAL LOS DOCUMENTOS QUE SE ENCUENTRAN EN LA RAIZ

}

const enum MODE_t Interface::addProfile(void){


  Cursor cursor();
  //String name = cursor.writer_ptr(); DESARROLLAR ESTA PARTE IGUAL...
  //Profiles::createProfile_(cursor.write_ptr());


}

const enum MODE_t Interface::deleteProfile(void){

  Profiles::showProfiles_();  // ESCRIBE EN EL SERIAL LOS DOCUMENTOS QUE SE ENCUENTRAN EN LA RAIZ
  // if(/*PONER AQUI OPCIONES ONDA .txt que hay en el directorio*/ == 0){ Interface::nonProfiles() } else {continua} (DESARROLLAR IGUAL, HAY Q PENSARLO MAS A FONDO ESTA PARTE, PARA PODER VOLVER DIGO)
  Cursor cursor(/*PONER AQUI OPCIONES ONDA .txt que hay en el directorio*/);

}


