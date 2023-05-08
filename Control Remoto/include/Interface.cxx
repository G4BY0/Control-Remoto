//Copyright Grupo 7, Inc. All Rights Reserved.
/***********************************************
 * * * * * * * * * * * * * * * * * * * * * * * *
 * \file
 * Source code of members for Interface.h
 * 
 * * * * * * * * * * * * * * * * * * * * * * * *
***********************************************/

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

void Interface::profiles(void){
  

  const char** && names = Profiles::showProfiles_();
  
  if (names == nullptr) {
    Interface::nonSubProfiles();
    return; // El puntero es nulo, salir de la función
  }

  CursorV2 cursor(names,&display);

  const char* && selected = cursor.getSelectedOption();

  SubProfiles::showSubProfiles(selected);
  

}

void Interface::addProfile(void){


  Cursor cursor();
  Profiles::createProfile_(cursor.write_ptr());
  //cursor.writer_ptr(); DESARROLLAR ESTA PARTE IGUAL...


}

void Interface::deleteProfile(void){

  Profiles::showProfiles_(); 

  Cursor cursor();
  Profiles::deleteProfile_(cursor.write_ptr());

  // if(/*PONER AQUI OPCIONES ONDA .txt que hay en el directorio*/ == 0){ Interface::nonProfiles() } else {continua} (DESARROLLAR IGUAL, HAY Q PENSARLO MAS A FONDO ESTA PARTE, PARA PODER VOLVER DIGO)
 

}


void Interface::subProfiles(const char *profileName_){

  const char** && names = SubProfiles::showSubProfiles(profileName_);
  
  if (*names == nullptr) {
    Interface::nonSubProfiles();
    return; // El puntero es nulo, salir de la función
    
  }

  CursorV2 cursor(names,&display);

  const char* && selected = cursor.getSelectedOption();

  SubProfiles::showSubProfiles(selected);

}

void Interface::nonProfiles(void){

  Serial.println("No hay perfiles disponibles en la SD O hubo una obstruccion al intentar hacerlo...");

  display.setTextSize(1);
  display.setCursor(10,10);
  display.println("No hay perfiles");
  display.setCursor(10,20);
  display.println("Almacenados");

  display.setCursor(10,40);
  display.println("Pulsa boton: Back para");
  display.setCursor(10,50);
  display.println("Volver");

  display.display();

}

void Interface::nonSubProfiles(void){

  Serial.println("No hay sub-perfiles disponibles en la SD O hubo una obstruccion al intentar hacerlo...");

  display.setTextSize(1);
  display.setCursor(10,10);
  display.println("No hay sub-perfiles");
  display.setCursor(10,20);
  display.println("Almacenados");

  display.setCursor(10,40);
  display.println("Pulsa boton: Back para");
  display.setCursor(10,50);
  display.println("Volver");

  display.display();

}