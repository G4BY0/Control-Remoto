//Copyright Grupo 7, Inc. All Rights Reserved.
/***********************************************
 * * * * * * * * * * * * * * * * * * * * * * * *
 * \file
 * Source code of members for Interface.h
 * 
 * * * * * * * * * * * * * * * * * * * * * * * *
***********************************************/

#include "Interface.h"

//Objeto para el manejo del Display OLED
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


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


  return 15; //Si hay problemas retorna
  

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
  
  WritterV2 writter( &display );

  String profileName = writter.stringFinished();
  if(profileName.c_str() == nullptr) return; // Failure
  String subProfileName = writter.stringFinished(); //Agregado para que luego de haber creado un perfil, vaya dentro de este a crear un subperfil
  if(subProfileName.c_str() == nullptr) return; // Failure
  
  Profiles::createProfile_(profileName.c_str());

  display.clearDisplay();

  Receive_start();
  while(!Receive_check()){
    display.setCursor(10,10);
    display.print(F("Prepared to \n receive IR\n SIGNAL \n\n Waiting For \n Response..."));
  }
  SubProfiles::createSubProfile_(subProfileName.c_str() , storeCode() , profileName.c_str() ); //Agregado para que luego de haber creado un perfil, vaya dentro de este a crear un subperfil

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

  Serial.println(F("No hay perfiles disponibles en la SD O hubo una obstruccion al intentar hacerlo..."));

  display.setTextColor(SH110X_WHITE);
  display.setTextSize(1);
  display.setCursor(10,10);
  display.println(F("No profiles stored"));

  display.setCursor(15,30);
  display.println(F("Press any button"));
  display.setCursor(25,40);
  display.println(F("to turn back"));

  display.display();
  
  //Descomentar para que solo sea el boton BACK
  while( buttonState(PIN::Buttons::BACK) );

  #pragma region Blink_nonProfiles_PulseAnyBotton
  //Descomentar para que sea para cualquier boton
  for(  unsigned long TimeForBlink = millis()  ;
        buttonState(PIN::Buttons::BACK)     |
        buttonState(PIN::Buttons::ENTER)    |
        buttonState(PIN::Buttons::UP)       |
        buttonState(PIN::Buttons::DOWN)     |
        buttonState(PIN::Buttons::LEFT)     |
        buttonState(PIN::Buttons::RIGHT)    ;
     )
  {
    bool AlternatingBlink;
    if( (millis() - TimeForBlink) >= BLINK_TIME){
      TimeForBlink = millis();
      AlternatingBlink = !AlternatingBlink;
      display.setCursor(15,30);
      display.println(F("                "));
      display.setCursor(25,40);
      display.println(F("            "));
      display.display();

    } else {
      TimeForBlink = millis();
      AlternatingBlink = !AlternatingBlink;
      TimeForBlink = millis();
      display.setCursor(10,10);
      display.println(F("No profiles stored"));

      display.setCursor(15,30);
      display.println(F("Press any button"));
      display.setCursor(25,40);
      display.println(F("to turn back"));

    }
  }
  #pragma endregion // Blink_PulseAnyBotton

}

void Interface::nonSubProfiles(void){

  Serial.println(F("No hay sub-perfiles disponibles en la SD O hubo una obstruccion al intentar hacerlo..."));

  display.setTextColor(SH110X_WHITE);
  display.setTextSize(1);
  display.setCursor(5,10);
  display.println(F("No sub-profiles stored"));

  display.setCursor(15,30);
  display.println(F("Press any button"));
  display.setCursor(25,40);
  display.println(F("to turn back"));

  display.display();

  //Descomentar para que solo sea el boton BACK
  while( buttonState(PIN::Buttons::BACK) );

  #pragma region Blink_nonSubProfiles_PulseAnyBotton
  //Descomentar para que sea para cualquier boton
  for(  unsigned long TimeForBlink = millis()  ;
        buttonState(PIN::Buttons::BACK)     |
        buttonState(PIN::Buttons::ENTER)    |
        buttonState(PIN::Buttons::UP)       |
        buttonState(PIN::Buttons::DOWN)     |
        buttonState(PIN::Buttons::LEFT)     |
        buttonState(PIN::Buttons::RIGHT)    ;
     )
  {
    bool AlternatingBlink;
    if( (millis() - TimeForBlink) >= BLINK_TIME){
      TimeForBlink = millis();
      AlternatingBlink = !AlternatingBlink;
      display.setCursor(15,30);
      display.println(F("                "));
      display.setCursor(25,40);
      display.println(F("            "));
      display.display();

    } else {
      TimeForBlink = millis();
      AlternatingBlink = !AlternatingBlink;
      display.setCursor(5,10);
      display.println(F("No sub-profiles stored"));

      display.setCursor(15,30);
      display.println(F("Press any button"));
      display.setCursor(25,40);
      display.println(F("to turn back"));

    }
  }
  #pragma endregion // Blink_PulseAnyBotton

}

void Interface::createSubProfile(void){

  //Primero muestro en pantalla los perfiles
  char** namesProfile = Profiles::showProfiles_();

  if (namesProfile == nullptr) {
    Interface::nonProfiles();
    return; // El puntero es nulo, salir de la función
  }

  CursorV2 cursor(namesProfile,&display);

  const char* && profileSelected = cursor.getSelectedOption();

  WritterV2 writter(&display);

  Receive_start();
  if(waitingForIR()) return; //Failure, Retorna que el usuario canceló la recepcion de la señal
  
  SubProfiles::createSubProfile_(writter.stringFinished().c_str() , storeCode() , profileSelected ); //Agregado para que luego de haber creado un perfil, vaya dentro de este a crear un subperfil

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

  if(selectedProfile == nullptr) return; //Failure, el usuario cancelo la seleccion de perfiles

  names = SubProfiles::showSubProfiles(selectedProfile);
  
  CursorV2 cursor2(names,&display); 

  const char* && selectedSubProfile = cursor2.getSelectedOption();

  if(selectedSubProfile == nullptr) return; //Failure, el usuario cancelo la seleccion de subperfiles

  SubProfiles::deleteSubProfile(selectedProfile, selectedSubProfile);

}

bool Interface::waitingForIR(void){

  while(!Receive_check()){
    display.setCursor(10,10);
    display.print(F("Prepared to \n Receive IR\n SIGNAL. \n\n Waiting For \n Response... \n Press Any Botton \n To Cancel."));
    if(buttonState(PIN::Buttons::BACK)      |
       buttonState(PIN::Buttons::ENTER)     |
       buttonState(PIN::Buttons::UP)        |
       buttonState(PIN::Buttons::DOWN)      |
       buttonState(PIN::Buttons::LEFT)      |
       buttonState(PIN::Buttons::RIGHT))     return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;

}