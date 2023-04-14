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

enum MODE_t Interface::hub(void){

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

enum MODE_t Interface::profiles(void){
  
  Profiles::showProfiles_();  // ESCRIBE EN EL SERIAL LOS DOCUMENTOS QUE SE ENCUENTRAN EN LA RAIZ

}

enum MODE_t Interface::addProfile(void){

  Cursor cursor();
  //String name = cursor.writer_ptr(); DESARROLLAR ESTA PARTE IGUAL...
  //Profiles::createProfile_(cursor.write_ptr());

}

enum MODE_t Interface::deleteProfile(void){

  Profiles::showProfiles_();  // ESCRIBE EN EL SERIAL LOS DOCUMENTOS QUE SE ENCUENTRAN EN LA RAIZ
  // if(/*PONER AQUI OPCIONES ONDA .txt que hay en el directorio*/ == 0){ Interface::nonProfiles() } else {continua} (DESARROLLAR IGUAL, HAY Q PENSARLO MAS A FONDO ESTA PARTE, PARA PODER VOLVER DIGO)
  Cursor cursor(/*PONER AQUI OPCIONES ONDA .txt que hay en el directorio*/);

}


