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
  
  const char** optionsString = new char*[5];
  optionsString = {"PROFILES" , "ADD PROFILE" , "DELETE PROFILE" , "ADD SUBPROFILE" , "DELETE SUBPROFILE"};

  CursorV2 cursor(optionsString,&display);

  display.setCursor(LINE_STRING_X,LINE_STRING_Y[0]);
  display.print    ("  PROFILES"   );

  display.setCursor(LINE_STRING_X,LINE_STRING_Y[1]);
  display.print    ("  ADD PROFILE");

  display.setCursor(LINE_STRING_X,LINE_STRING_Y[2]);
  display.print    ("  DELETE PROFILE");

  display.display();

  
  return cursor.getSelectedOption();
  

}

void Interface::profiles(void){
  

  const char** && names = Profiles::showProfiles_();

  if (names == nullptr) {
    Interface::nonProfiles();
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

   const char** && names = Profiles::showProfiles_();
  
  if (names == nullptr) {
    Interface::nonSubProfiles();
    return; // El puntero es nulo, salir de la función
  }

  CursorV2 cursor(names,&display);

  const char* && selected = cursor.getSelectedOption();

  Profiles::deleteProfile_(selected);


  // if(/*PONER AQUI OPCIONES ONDA .txt que hay en el directorio*/ == 0){ Interface::nonProfiles() } else {continua} (DESARROLLAR IGUAL, HAY Q PENSARLO MAS A FONDO ESTA PARTE, PARA PODER VOLVER DIGO)
 

}


void Interface::subProfiles(const char *profileName_){

  const char** && names = SubProfiles::showSubProfiles(profileName_);
  
  if (names == nullptr) {
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
  display.println("Pulsa boton Back para");
  display.setCursor(10,50);
  display.println("Volver");

  display.display();
  while(buttonState(PIN::Buttons::BACK));

}

void Interface::nonSubProfiles(void){

  Serial.println("No hay sub-perfiles disponibles en la SD O hubo una obstruccion al intentar hacerlo...");

  display.setTextSize(1);
  display.setCursor(10,10);
  display.println("No hay sub-perfiles");
  display.setCursor(10,20);
  display.println("Almacenados");

  display.setCursor(10,40);
  display.println("Pulsa boton Back para");
  display.setCursor(10,50);
  display.println("Volver");

  display.display();
  while(buttonState(PIN::Buttons::BACK));

}

void Interface::createSubProfile(void){

  //Primero muestro en pantalla los perfiles
  const char** && namesProfile = Profiles::showProfiles_();

  if (namesProfile == nullptr) {
    Interface::nonProfiles();
    return; // El puntero es nulo, salir de la función
  }

  CursorV2 cursor(namesProfile,&display);

  const char* && selected = cursor.getSelectedOption();

  const char* && namesSubProfile = SubProfiles::showSubProfiles(selected);

  CursorV2 cursor2(namesProfile,&display); 

  const char* && selected = cursor2.getSelectedOption();



}

void Interface::deleteSubProfile(void){

  //Primero muestro en pantalla los perfiles
  const char** && names = Profiles::showProfiles_();

  if (names == nullptr) {
    Interface::nonProfiles();
    return; // El puntero es nulo, salir de la función
  }

  CursorV2 cursor(names,&display);

  const char* && selected = cursor.getSelectedOption();

  names = SubProfiles::showSubProfiles(selected);

  CursorV2 cursor2(names,&display); 

  selected = cursor2.getSelectedOption();

  SubProfiles::deleteSubProfile();

}