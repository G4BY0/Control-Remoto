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


void displayBegin(void){

  display.begin(I2C_ADDRESS, true);
  display.display();
  display.clearDisplay();
  display.display();

}

uint8_t Interface::hub(void){

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
  return cursor.options();
  

}

boolean Interface::profiles(void){
  
  Profiles::showProfiles_();  // ESCRIBE EN EL SERIAL LOS DOCUMENTOS QUE SE ENCUENTRAN EN LA RAIZ

}

boolean Interface::addProfile(void){

  Cursor cursor();
  //String name = cursor.writer_ptr(); DESARROLLAR ESTA PARTE IGUAL...
  //Profiles::createProfile_(cursor.write_ptr());

}

boolean Interface::deleteProfile(void){

  Profiles::showProfiles_();  // ESCRIBE EN EL SERIAL LOS DOCUMENTOS QUE SE ENCUENTRAN EN LA RAIZ
  // if(/*PONER AQUI OPCIONES ONDA .txt que hay en el directorio*/ == 0){ Interface::nonProfiles() } else {continua} (DESARROLLAR IGUAL, HAY Q PENSARLO MAS A FONDO ESTA PARTE, PARA PODER VOLVER DIGO)
  Cursor cursor(/*PONER AQUI OPCIONES ONDA .txt que hay en el directorio*/);

}


