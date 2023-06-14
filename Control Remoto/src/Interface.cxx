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

  pinMode(PIN::Buttons::UP,     INPUT_PULLUP);
  pinMode(PIN::Buttons::DOWN,   INPUT_PULLUP);
  pinMode(PIN::Buttons::LEFT ,  INPUT_PULLUP);
  pinMode(PIN::Buttons::RIGHT , INPUT_PULLUP);
  pinMode(PIN::Buttons::BACK ,  INPUT_PULLUP);
  pinMode(PIN::Buttons::ENTER,  INPUT_PULLUP);  

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
  Serial.println(cursor.getSelectedOption());
  const char* && selected = cursor.getSelectedOption();

  const uint8_t && CantidadDeStrings = sizeof(strings) / sizeof(strings[0]); 

  for(uint8_t option = 0 ; option < CantidadDeStrings ; option++){ 
    if(strcmp( selected , optionsString[option] ) == 0) 
    return option; 
  }


  return 0; //Si hay problemas retorna
  

}

void Interface::profiles(void){
  

  char** profiles_ptr = Profiles::showProfiles_();

  if ( profiles_ptr == nullptr) {
    Interface::nonProfiles();
    return; // El puntero es nulo, salir de la función
  }

  CursorV2 cursor( profiles_ptr , &display );

  //delete[] profiles_ptr; // Borro Los datos de los perfiles ya que estan guardados en el heap (memoria )

  const char* profile_selected = cursor.getSelectedOption();
  if(profile_selected == nullptr) return;

  Interface::subProfiles( profile_selected );
  

}

void Interface::addProfile(void){
  
  WritterV2 writter( &display );
  #pragma region Debugging
  Serial.println(F("Llegue hasta aca"));
  #pragma endregion
  String profileName = writter.stringFinished();
  if((profileName.c_str() == nullptr)  || profileName.c_str() == NULL) return; // Failure
  String subProfileName = writter.stringFinished(); //Agregado para que luego de haber creado un perfil, vaya dentro de este a crear un subperfil
  if(subProfileName == nullptr ) return; // Failure
  
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

  char** subprofiles_ptr = SubProfiles::showSubProfiles(profileName_);
  
  if (subprofiles_ptr == nullptr) {
    Interface::nonSubProfiles();
    return; // El puntero es nulo, salir de la función
    
  }

  CursorV2 cursor( subprofiles_ptr ,&display);

  const char* && subprofiles_selected = cursor.getSelectedOption();
  if(subprofiles_selected == nullptr) return;
  
  // Desarrollar a infrarrojo el subperfil solicitado
  Keep_t* && IRToSend = SubProfiles::ReturnSubProfile( profileName_ , subprofiles_selected );

  if(IRToSend == nullptr ){
    Serial.println(F("The IR Signal can´t be send because has been received wrong IRDATA"));
    return;
  }

  sendCode(IRToSend);

}

void Interface::nonProfiles(void){

  display.clearDisplay();
  display.display();

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
  
  while(  buttonState(PIN::Buttons::BACK)  == HIGH  ||
          buttonState(PIN::Buttons::UP)    == HIGH  ||
          buttonState(PIN::Buttons::DOWN)  == HIGH  ||
          buttonState(PIN::Buttons::LEFT)  == HIGH  ||
          buttonState(PIN::Buttons::RIGHT) == HIGH  || 
          buttonState(PIN::Buttons::ENTER) == HIGH    );
  delay(DEBOUNCE_TIME);  // DELAY PARA EL REBOTE DEL PULSADOR DE FENOMENO MECANICO

}

void Interface::nonSubProfiles(void){

  display.clearDisplay();
  display.display();

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

  while(  buttonState(PIN::Buttons::BACK)  == HIGH  ||
          buttonState(PIN::Buttons::UP)    == HIGH  ||
          buttonState(PIN::Buttons::DOWN)  == HIGH  ||
          buttonState(PIN::Buttons::LEFT)  == HIGH  ||
          buttonState(PIN::Buttons::RIGHT) == HIGH  || 
          buttonState(PIN::Buttons::ENTER) == HIGH    );
  delay(DEBOUNCE_TIME);  // DELAY PARA EL REBOTE DEL PULSADOR DE FENOMENO MECANICO
  delay(DEBOUNCE_TIME); // DELAY PARA EL REBOTE DEL PULSADOR DE FENOMENO MECANICO

}

//void Interface::createSubProfile(char** namesProfile = nullptr)
void Interface::createSubProfile(void){

  //Primero muestro en pantalla los perfiles
  char** namesProfile = Profiles::showProfiles_();

  if ( namesProfile == nullptr ) {
    Interface::nonProfiles();
    return; // El puntero es nulo, salir de la función
  }

  CursorV2 cursor(namesProfile,&display);

  const char* && profileSelected = cursor.getSelectedOption();

  WritterV2 writter(&display);

  Receive_start();
  if(waitingForIR()) return; //Failure, Retorna que el usuario canceló la recepcion de la señal
  
  SubProfiles::createSubProfile_(writter.stringFinished() , storeCode() , profileSelected ); //Agregado para que luego de haber creado un perfil, vaya dentro de este a crear un subperfil

}

//void Interface::createSubProfile(char** names = nullptr)
void Interface::deleteSubProfile(void){

  //Primero muestro en pantalla los perfiles
  char** names = Profiles::showProfiles_();

  if ( names == nullptr ) {
    Interface::nonProfiles();
    return; // El puntero es nulo, salir de la función
  }

  CursorV2 cursor( names ,&display);

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

    //Logica de si se llegara a presionar algun boton
    if( buttonState(PIN::Buttons::BACK)   ||
        buttonState(PIN::Buttons::UP)     ||
        buttonState(PIN::Buttons::DOWN)   ||
        buttonState(PIN::Buttons::LEFT)   ||
        buttonState(PIN::Buttons::RIGHT)  ||
        buttonState(PIN::Buttons::ENTER)    ) return EXIT_FAILURE;

    Receive_start();

    if(Receive_check()){
      Receive_stop();
      continue;
    }

    Receive_stop();
  }

  return EXIT_SUCCESS;

}