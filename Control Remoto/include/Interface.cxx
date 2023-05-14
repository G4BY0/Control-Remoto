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
  
  char* strings[] = {"PROFILES" , "ADD PROFILE" , "DELETE PROFILE" , "ADD SUBPROFILE" , "DELETE SUBPROFILE"};
  char** optionsString = strings;

  CursorV2 cursor(optionsString,&display);
  
  const char* && selected = cursor.getSelectedOption();

  for(uint8_t option; option < 5; option++){ if(strcmp( selected , optionsString[option] ) == 0) return option; }


  return; //Si hay problemas retorna
  

}

void Interface::profiles(void){
  

  char** names = Profiles::showProfiles_();

  if (names == nullptr) {
    Interface::nonProfiles();
    return; // El puntero es nulo, salir de la función
  }

  CursorV2 cursor(names,&display);

  const char* && selected = cursor.getSelectedOption();

  Interface::subProfiles(selected);
  

}

void Interface::addProfile(void){
  
  Writter writter( &display );

  String profileName = writter.stringFinished();
  if(profileName.c_str() == nullptr) return; // Failure
  String subProfileName = writter.stringFinished(); //Agregado para que luego de haber creado un perfil, vaya dentro de este a crear un subperfil
  if(subProfileName.c_str() == nullptr) return; // Failure
  
  Profiles::createProfile_(profileName.c_str());
  SubProfiles::createSubProfile_(subProfileName.c_str() , ReceivingAndStoring( profileName.c_str() , subProfileName.c_str() ) , profileName.c_str() ); //Agregado para que luego de haber creado un perfil, vaya dentro de este a crear un subperfil

}

void Interface::deleteProfile(void){

  char** names = Profiles::showProfiles_();
  
  if (names == nullptr) {
    Interface::nonSubProfiles();
    return; // El puntero es nulo, salir de la función
  }

  CursorV2 cursor(names,&display);

  const char* && selected = cursor.getSelectedOption();

  Profiles::deleteProfile_(selected);

}

void Interface::subProfiles(const char *profileName_){

  char** names = SubProfiles::showSubProfiles(profileName_);
  
  if (names == nullptr) {
    Interface::nonSubProfiles();
    return; // El puntero es nulo, salir de la función
    
  }

  CursorV2 cursor( names ,&display);

  const char* && selected = cursor.getSelectedOption();

  // Desarrollar a infrarrojo el subperfil solicitado

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
  
  //Descomentar para que solo sea el boton BACK
  while( buttonState(PIN::Buttons::BACK) );

  //Descomentar para que sea para cualquier boton
  /*while(  buttonState(PIN::Buttons::BACK)  ||
            buttonState(PIN::Buttons::ENTER) ||
            buttonState(PIN::Buttons::UP)    ||
            buttonState(PIN::Buttons::DOWN)  ||
            buttonState(PIN::Buttons::LEFT)  ||
            buttonState(PIN::Buttons::RIGHT) );
  */

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

  //Descomentar para que solo sea el boton BACK
  while( buttonState(PIN::Buttons::BACK) );

  //Descomentar para que sea para cualquier boton
  /*while(  buttonState(PIN::Buttons::BACK)  ||
            buttonState(PIN::Buttons::ENTER) ||
            buttonState(PIN::Buttons::UP)    ||
            buttonState(PIN::Buttons::DOWN)  ||
            buttonState(PIN::Buttons::LEFT)  ||
            buttonState(PIN::Buttons::RIGHT) );
  */

}

void Interface::createSubProfile(void){

  //Primero muestro en pantalla los perfiles
  char** namesProfile = Profiles::showProfiles_();

  if (namesProfile == nullptr) {
    Interface::nonProfiles();
    return; // El puntero es nulo, salir de la función
  }

  CursorV2 cursor(namesProfile,&display);

  const char* && selected = cursor.getSelectedOption();

  const char** && namesSubProfile = SubProfiles::showSubProfiles(selected);

  CursorV2 cursor2(namesProfile,&display); 

  const char* && selected = cursor2.getSelectedOption();

}

void Interface::deleteSubProfile(void){

  //Primero muestro en pantalla los perfiles
  char** names = Profiles::showProfiles_();

  if (names == nullptr) {
    Interface::nonProfiles();
    return; // El puntero es nulo, salir de la función
  }

  CursorV2 cursor(names,&display);

  const char* && selectedProfile = cursor.getSelectedOption();

  names = SubProfiles::showSubProfiles(selectedProfile);
  
  CursorV2 cursor2(names,&display); 

  const char* && selectedSubProfile = cursor2.getSelectedOption();

  SubProfiles::deleteSubProfile(selectedProfile, selectedSubProfile);

}