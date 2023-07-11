//Copyright Grupo 11, Inc. All Rights Reserved.
/***********************************************
 * * * * * * * * * * * * * * * * * * * * * * * *
 * \file
 * Source code of members for Interface.h
 * 
 * * * * * * * * * * * * * * * * * * * * * * * *
***********************************************/
#include "Interface.h"

//Objeto para el manejo de la pantalla a utilizar (tecnologia OLED con driver SH1106G)
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
//Semaforo para el libre uso del display
SemaphoreHandle_t semaphoreDisplay;  

void buttonsBegin(void){

  //Configuracion PULLUP
  pinMode(PIN::Buttons::UP,     INPUT_PULLUP);  //Boton UP
  //Configuracion PULLUP
  pinMode(PIN::Buttons::DOWN,   INPUT_PULLUP);  //Boton DOWN
  //Configuracion PULLUP
  pinMode(PIN::Buttons::LEFT ,  INPUT_PULLUP);  //Boton LEFT
  //Configuracion PULLUP
  pinMode(PIN::Buttons::RIGHT , INPUT_PULLUP);  //Boton RIGHT
  //Configuracion PULLUP
  pinMode(PIN::Buttons::BACK ,  INPUT_PULLUP);  //Boton BACK
  //Configuracion PULLUP
  pinMode(PIN::Buttons::ENTER,  INPUT_PULLUP);  //Boton ENTER

}

void displayBegin(void){

  // Inicializacion del Semaforo
  semaphoreDisplay = xSemaphoreCreateBinary();
  xSemaphoreGive(semaphoreDisplay); //Inicializado el semaforo

  // Bloquear el semáforo
  xSemaphoreTake( semaphoreDisplay , portMAX_DELAY );

  //Inicializacion del display con la comunicacion I2C
  display.begin(I2C_ADDRESS, true);

  //Expulso de la RAM hacia el display las instrucciones guardadas
  display.display();

  //Limpio la pantalla porque al inicio mete una imagen el buffer de entrada
  display.clearDisplay();
  display.display();

  //Establezco los parametros a utilizar para la muestra a la salida del display
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.display();

  //Inicializado el semaforo
  xSemaphoreGive(semaphoreDisplay);

}

//DEBUGGING CURSOR
uint8_t Interface::hub(void){
  
  //Opciones del menu Principal/Hub
  std::vector<std::string> strings  = { "PROFILES" , "ADD PROFILE" , "DELETE PROFILE" , "ADD SUBPROFILE" , "DELETE SUBPROFILE" , "HELP" , "SHUTDOWN" };

  Cursor cursor(strings,&display);
  const char* selected;

  do{ selected = cursor.getSelectedOption(); } 
  while(selected == nullptr);

  //Busco el numero de la opcion seleccionada y lo retorno
  for(uint8_t iterator = 0; iterator < strings.size(); iterator++ ){
    if(strcmp( selected , strings[iterator].c_str() ) == 0) 
    return iterator; 
  }

}
//---------------------------------------------------

void Interface::profiles(void){
  
  //Recibo Vector de Strings con el nombre de cada perfil cada uno (Ignorando el archivo reservado "Transfer.db")
  auto profiles_ptr = Profiles::showProfiles_();

  if ( profiles_ptr.empty() == true) {
    Interface::EmergencyCalls::nonProfiles();
    return; // El puntero es nulo, salir de la función
  }

  profiles_ptr.insert( profiles_ptr.begin() , "ADD PROFILES" ); //Primera Opcion
  profiles_ptr.insert( profiles_ptr.begin() +1 , "DELETE PROFILES" );  //Segunda Opcion

  Cursor cursor( profiles_ptr , &display );
  const char* profile_selected = cursor.getSelectedOption();
  
  //Si el usuario no selecciono nada o hubo algun problema
  if(profile_selected == nullptr) return;

  if(profile_selected == "ADD PROFILES"){
  Interface::addProfile();
  return;
  }

  if(profile_selected == "DELETE PROFILES"){
  Interface::deleteProfile();
  return;
  }

  //Muestro los subperfiles del perfil seleccionado por el usuario
  Interface::subProfiles( profile_selected );

}

void Interface::addProfile(void){

  //Inicializo un Writter para recibir del usuario el nombre del nuevo perfil
  WritterV2 writter( &display );

  String profileName = writter.stringFinished();
  delay(DEBOUNCE_TIME);
  
  //En caso de cualquier problema incluyendo si el usuario cancelo la creacion de un nuevo perfil
  if( profileName.c_str() == nullptr ) return; //Failure, el usuario habia cancelado la escritura del nombre 

  //Creo el perfil en el almacenamiento con el nombre dado Y pregunto si no se cumplió correctamente...
  if(! Profiles::createProfile_(profileName.c_str()) ) {
    Interface::EmergencyCalls::noProfileCreated();
    return;
  }

  // Bloquear el semáforo
  xSemaphoreTake( semaphoreDisplay , portMAX_DELAY );

  //Pantalla Emergente que le pregunta al usuario si desea agregar un subperfil en este momento
  //Establezco los parametros a utilizar para la muestra a la salida del display
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);

  //Le digo al usuario por pantalla si desea agregar un Subperfil en este momento
  display.setCursor(5,10);
  display.println(F("Do you want to add"));
  display.setCursor(6,20);
  display.println(F("a Subprofile Now?"));
  display.setCursor(20,30);
  display.println(F("Press Enter To "));
  display.setCursor(10,40);
  display.println(F("Continue. Press any"));
  display.setCursor(20,50);
  display.println(F("other to Cancel"));

  display.display();

  // Desbloquear el semáforo
  xSemaphoreGive(semaphoreDisplay);

  //Hasta que no haya una respuesta de los pulsadores (ENTER para continuar y los demas para Cancelar)
  while(1){
    if(buttonState(PIN::Buttons::ENTER) == HIGH ){
      delay(DEBOUNCE_TIME);  // DELAY PARA EL REBOTE DEL PULSADOR DE FENOMENO MECANICO
      break;
    } 
    else if( buttonState(PIN::Buttons::BACK)  == HIGH  ||
             buttonState(PIN::Buttons::UP)    == HIGH  ||
             buttonState(PIN::Buttons::DOWN)  == HIGH  ||
             buttonState(PIN::Buttons::LEFT)  == HIGH  ||
             buttonState(PIN::Buttons::RIGHT) == HIGH    ){
      delay(DEBOUNCE_TIME);  // DELAY PARA EL REBOTE DEL PULSADOR DE FENOMENO MECANICO
      return;
    }
  }

  if(waitingForIR() == EXIT_FAILURE) return; //Si se cancela o se recibe un error al recibir la señal infrarroja (Se almacena en el objeto global "IrReceiver")
  yield();

  //Recbido del usuario (A traves del writter) el nombre del nuevo subperfil
  std::string subProfileName = writter.stringFinished(); //Recibo el nombre del subperfil seleccionado por el usuario
  if(subProfileName.c_str() == nullptr ) return; // SI no recibo ningun nombre...

  
  //Crea un subperfil para la señal recibida con: la señal recibida y los nombres escritos por el usuario
  SubProfiles::createSubProfile_(  subProfileName.c_str() , Protocols::IR , profileName.c_str() ); 

}

void Interface::deleteProfile(void){

  //Recibo del almacenamiento un vector de strings de los nombres de los perfiles
  auto names = Profiles::showProfiles_();
  
  //Si recibo el vector vacio, hubo un problema
  if (names.empty() == true) {
    Interface::EmergencyCalls::nonProfiles();
    return; // El puntero es nulo, salir de la función
  }

  //Inicializo un Cursor para recibir del usuario el perfil seleccionado
  Cursor cursor(names,&display);
  const char* selected = cursor.getSelectedOption();

  //Pido al almacenamiento eliminar el perfil dado
  Profiles::deleteProfile_(selected);

}

void Interface::subProfiles(const char *profileName_){

  // Pido del almacenamiento los nombres de los subperfiles del perfil dado
  auto subprofiles = SubProfiles::showSubProfiles(profileName_); 
  
  subprofiles.insert(  subprofiles.begin() , "ADD SUBPROFILE" ); // Primera Opcion
  subprofiles.insert(  subprofiles.begin()+1 , "DELETE SUBPROFILE" ); // Segunda Opcion

  // Inicializo Un cursor para pedir al usuario que subperfil desea seleccionar
  Cursor cursor( subprofiles ,&display); 
  const char* subprofiles_selected = cursor.getSelectedOption();
  if(subprofiles_selected == nullptr) return; // Si no se selecciono ninguno...

  if(subprofiles_selected == "ADD SUBPROFILE"){
  Interface::createSubProfile(profileName_);
  return;
  }

  if(subprofiles_selected == "DELETE SUBPROFILE"){
  Interface::deleteSubProfile(profileName_);
  return;
  }

  // Informacion a enviar a la salida
  auto IRToSend = SubProfiles::ReturnSubProfile( profileName_ , subprofiles_selected ); //Pido del almacenamiento la informacion de la señal a transmitir del subperfil dado
  if( IRToSend == nullptr ){ // Si no se encuentra almacenada o hubo un error inesperado...
    Serial.println(F("The IR Signal can´t be send because has been received wrong IRDATA"));
    return;
  }
  
  sendCode(IRToSend); //Envio la señal a la salida con la informacion dada 

}

void Interface::createSubProfile(const char* profileSelected = nullptr){

  if(profileSelected == nullptr){

    auto namesProfile = Profiles::showProfiles_(); // Recibo del almacenamiento todos los nombres de los perfiles

    //Si no llegara a recibir ningun nombre de ningun perfil o si hubo un error inesperado...
    if ( namesProfile.empty() == true ) {
      Interface::EmergencyCalls::nonProfiles();
      return; 
    }

    //Inicializo un cursor para preguntar en que perfil desea almacenar el nuevo subperfil
    Cursor cursor(namesProfile,&display);
    const char* profileSelected = cursor.getSelectedOption();

  }

  //Pido la informacion a la entrada
  if(!waitingForIR()) return; //Failure, Retorna que el usuario canceló la recepcion de la señal.

  //Inicializo un Writter para pedirle al usuario el nombre del nuevo subperfil
  WritterV2 writter(&display);

  //Creo en el almacenamiento el nuevo subperfil en el perfil junto a la informacion
  SubProfiles::createSubProfile_(  writter.stringFinished() , Protocols::IR , profileSelected ); 
  
}

void Interface::deleteSubProfile(const char* profileSelected = nullptr){

  if(profileSelected == nullptr){

  //Recibo del almacenamiento el nombre de los perfiles
  auto namesProfiles = Profiles::showProfiles_();

  if ( namesProfiles.empty() == true ) { //Si no recibo ningun nombre...
    Interface::EmergencyCalls::nonProfiles();
    return; // El puntero es nulo, salir de la función
  }

  //Inicializo un cursor para pedirle al usuario que perfil desea
  Cursor cursor( namesProfiles , &display );
  const char* && profileSelected = cursor.getSelectedOption();

  }

  //Si no recibo ningun nombre...
  if(profileSelected == nullptr) return; //Failure, el usuario cancelo la seleccion de perfiles

  //Recido del almacenamiento el nombre de los subperfiles del perfil dado
  auto namesSubProfiles = SubProfiles::showSubProfiles(profileSelected);
  
  //Inicializo un cursor para pedirle al usuario que SUbperfil desea
  Cursor cursor2( namesSubProfiles , &display ); 
  const char* && selectedSubProfile = cursor2.getSelectedOption();

  //Si no recibo ningun Subperfil...
  if(selectedSubProfile == nullptr) return; //Failure, el usuario cancelo la seleccion de subperfiles

  //Elimino el subperfil dado del subperfil dado
  SubProfiles::deleteSubProfile(profileSelected, selectedSubProfile);

}

bool Interface::waitingForIR(void){

  // Bloquear el semáforo
  xSemaphoreTake( semaphoreDisplay , portMAX_DELAY );

  //Le hago saber al usuario por pantalla de que se esta esperando respuesta de la informacion
  //Establezco los parametros a utilizar para la muestra a la salida del display
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);

  display.setCursor(0,5);
  display.print(F("Prepared to Receive\nIR SIGNAL.\nWaiting For\nResponse... \nPress Any Botton\nTo Cancel."));
  display.display();

  // Desbloquear el semáforo
  xSemaphoreGive( semaphoreDisplay );

  //Inicializo la entrada para recibir la informacion
  Serial.println(F("Start Receiving for infrared signals."));
  Receive_start();
  //Mientras el codigo recibido sea invalido...
  while( !Receive_check() ){
    //Logica de si se llegara a presionar algun boton
    if( buttonState(PIN::Buttons::BACK)   ||
        buttonState(PIN::Buttons::UP)     ||
        buttonState(PIN::Buttons::DOWN)   ||
        buttonState(PIN::Buttons::LEFT)   ||
        buttonState(PIN::Buttons::RIGHT)  ||
        buttonState(PIN::Buttons::ENTER)    ) {
      Receive_stop();
      Serial.println(F("End Receiving for infrared signals."));
      delay(DEBOUNCE_TIME); //Rebote del fenomeno del pulsador
      return false; //Failure, se cancelo
      }
  }
  Receive_stop();
  Serial.println(F("End Receiving for infrared signals."));
  delay(DEBOUNCE_TIME); //Rebote del fenomeno del pulsador
  return true; //Recibido Correctamente

}

void Interface::help(const char* text , uint8_t version = 4){

  // Bloquear el semáforo
  xSemaphoreTake( semaphoreDisplay , portMAX_DELAY );

  //Establezco los parametros a utilizar para la muestra a la salida del display
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.display();
  
  QRCode qrcode;
  uint8_t qrcodeData[qrcode_getBufferSize(version)];
  qrcode_initText(&qrcode , qrcodeData , version , 0 , text );

  int scale = min( SCREEN_WIDTH / qrcode.size , SCREEN_HEIGHT / qrcode.size ) * 2; 

  int shiftX = ( SCREEN_WIDTH - qrcode.size * scale ) / 2;
  int shiftY = ( SCREEN_HEIGHT - qrcode.size * scale ) / 2;

  for(uint8_t y = 0 ; y < qrcode.size ; y++){
    for(uint8_t x = 0 ; x < qrcode.size ; x++){
      if(qrcode_getModule(&qrcode , x , y ))
        display.fillRect( shiftX + x * scale , shiftY + y * scale , scale , scale , SH110X_WHITE );
    }

  }
  display.display();

  // Desbloquear el semáforo
  xSemaphoreGive( semaphoreDisplay );

  //Si se presiona cualquier boton...
  while(!(buttonState(PIN::Buttons::BACK)  == HIGH  ||
          buttonState(PIN::Buttons::UP)    == HIGH  ||
          buttonState(PIN::Buttons::DOWN)  == HIGH  ||
          buttonState(PIN::Buttons::LEFT)  == HIGH  ||
          buttonState(PIN::Buttons::RIGHT) == HIGH  || 
          buttonState(PIN::Buttons::ENTER) == HIGH    ));
  delay(DEBOUNCE_TIME);  // DELAY PARA EL REBOTE DEL PULSADOR DE FENOMENO MECANICO  
}

void Interface::battery(void){

  PROGMEM const uint8_t batt_high_bits[] = {
    0xFF, 0x7F, 0x01, 0x40, 0xFD, 0xC3, 0xFD, 0xC3, 0xFD, 0xC3, 0xFD, 0xC3, 0x01, 0x40, 0xFF, 0x7F,
  };

  const uint8_t && battery_level = map(analogRead(PIN::Energy::BATTERY) , 0 , 1024 , 1 , 3 );

  // Bloquear el semáforo
  xSemaphoreTake( semaphoreDisplay , portMAX_DELAY );

  switch( battery_level ){

    case 1: 
    break;

    case 2: 
    break;

    case 3:
    display.drawBitmap( 118 , 10 , batt_high_bits , 10 , 5 , SH110X_WHITE );
  
    break;

  }

  // Desbloquear el semáforo
  xSemaphoreGive( semaphoreDisplay );


}

void Interface::EmergencyCalls::nonProfiles(void){

  // Bloquear el semáforo
  xSemaphoreTake( semaphoreDisplay, portMAX_DELAY );

  //Establezco los parametros a utilizar para la muestra a la salida del display
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.display();

  //Le muestro al usuario el problema que surgio
  Serial.println(F("No hay perfiles disponibles en la SD O hubo una obstruccion al intentar hacerlo..."));
  display.setCursor(10,15);
  display.println(F("No profiles stored"));

  //Le digo al usuario como terminar la ventana emergente
  display.setCursor(15,40);
  display.println(F("Press any button"));
  display.setCursor(25,50);
  display.println(F("to turn back"));

  display.display();
  
  // Desbloquear el semáforo
  xSemaphoreGive(semaphoreDisplay);

  //Logica de si se llegara a pulsar cualquier boton
  while(!(  buttonState(PIN::Buttons::BACK ) == HIGH  ||
            buttonState(PIN::Buttons::UP   ) == HIGH  ||
            buttonState(PIN::Buttons::DOWN ) == HIGH  ||
            buttonState(PIN::Buttons::LEFT ) == HIGH  ||
            buttonState(PIN::Buttons::RIGHT) == HIGH  || 
            buttonState(PIN::Buttons::ENTER) == HIGH     ));
  delay(DEBOUNCE_TIME);  // delay para evitar pulsaciones de mas de las que el usuario no hizo voluntariamente

}

void Interface::EmergencyCalls::nonSubProfiles(void){

  // Bloquear el semáforo
  xSemaphoreTake( semaphoreDisplay, portMAX_DELAY );

  //Establezco los parametros a utilizar para la muestra a la salida del display
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.display();

  //Le muestro al usuario el problema que surgio
  Serial.println(F("No hay sub-perfiles disponibles en la SD O hubo una obstruccion al intentar hacerlo..."));
  display.setCursor(20,10);
  display.println(F("No sub-profiles"));
  display.setCursor(40,20);
  display.println(F("Stored"));
  //Le digo al usuario como terminar la ventana emergente
  display.setCursor(15,40);
  display.println(F("Press any button"));
  display.setCursor(25,50);
  display.println(F("to turn back"));

  display.display();
  
  // Desbloquear el semáforo
  xSemaphoreGive(semaphoreDisplay);

  //Logica de si se llegara a pulsar cualquier boton
  while(!(  buttonState(PIN::Buttons::BACK ) == HIGH  ||
            buttonState(PIN::Buttons::UP   ) == HIGH  ||
            buttonState(PIN::Buttons::DOWN ) == HIGH  ||
            buttonState(PIN::Buttons::LEFT ) == HIGH  ||
            buttonState(PIN::Buttons::RIGHT) == HIGH  || 
            buttonState(PIN::Buttons::ENTER) == HIGH     ));
  delay(DEBOUNCE_TIME);  // DELAY PARA EL REBOTE DEL PULSADOR DE FENOMENO MECANICO

}

void Interface::EmergencyCalls::noProfileCreated(void){

  // Bloquear el semáforo
  xSemaphoreTake( semaphoreDisplay, portMAX_DELAY );

  //Establezco los parametros a utilizar para la muestra a la salida del display
  display.clearDisplay();
  display.setTextColor(SH110X_WHITE);
  display.setTextSize(1);

  display.setCursor(30,10);
  display.println(F("Couldn't Succeded "));
  display.setCursor(20,20);
  display.println(F("Creating The Profile"));

  //Le digo al usuario como terminar la ventana emergente
  display.setCursor(15,40);
  display.println(F("Press any button"));
  display.setCursor(25,50);
  display.println(F("to turn back"));

  display.display();
  
  // Desbloquear el semáforo
  xSemaphoreGive(semaphoreDisplay);

  //Logica de si se llegara a pulsar cualquier boton
  while(!(  buttonState(PIN::Buttons::BACK ) == HIGH  ||
            buttonState(PIN::Buttons::UP   ) == HIGH  ||
            buttonState(PIN::Buttons::DOWN ) == HIGH  ||
            buttonState(PIN::Buttons::LEFT ) == HIGH  ||
            buttonState(PIN::Buttons::RIGHT) == HIGH  || 
            buttonState(PIN::Buttons::ENTER) == HIGH     ));
  delay(DEBOUNCE_TIME);  // delay para evitar pulsaciones de mas de las que el usuario no hizo voluntariamente

}

void Interface::EmergencyCalls::noSubProfileCreated(void){

  // Bloquear el semáforo
  xSemaphoreTake( semaphoreDisplay, portMAX_DELAY );

  //Establezco los parametros a utilizar para la muestra a la salida del display
  display.clearDisplay();
  display.setTextColor(SH110X_WHITE);
  display.setTextSize(1);

  display.setCursor(30,10);
  display.println(F("Couldn't Succeded "));
  display.setCursor(10,20);
  display.println(F("Creating The Sub-Profile"));

  //Le digo al usuario como terminar la ventana emergente
  display.setCursor(15,40);
  display.println(F("Press any button"));
  display.setCursor(25,50);
  display.println(F("to turn back"));

  display.display();
  
  // Desbloquear el semáforo
  xSemaphoreGive(semaphoreDisplay);

  //Logica de si se llegara a pulsar cualquier boton
  while(!(  buttonState(PIN::Buttons::BACK ) == HIGH  ||
            buttonState(PIN::Buttons::UP   ) == HIGH  ||
            buttonState(PIN::Buttons::DOWN ) == HIGH  ||
            buttonState(PIN::Buttons::LEFT ) == HIGH  ||
            buttonState(PIN::Buttons::RIGHT) == HIGH  || 
            buttonState(PIN::Buttons::ENTER) == HIGH     ));
  delay(DEBOUNCE_TIME);  // delay para evitar pulsaciones de mas de las que el usuario no hizo voluntariamente

}

void Interface::EmergencyCalls::noProfileDeleted(void){

  // Bloquear el semáforo
  xSemaphoreTake( semaphoreDisplay, portMAX_DELAY );

  //Establezco los parametros a utilizar para la muestra a la salida del display
  display.clearDisplay();
  display.setTextColor(SH110X_WHITE);
  display.setTextSize(1);

  display.setCursor(30,10);
  display.println(F("Couldn't Succeded "));
  display.setCursor(20,20);
  display.println(F("Deleting The Profile"));

  //Le digo al usuario como terminar la ventana emergente
  display.setCursor(15,40);
  display.println(F("Press any button"));
  display.setCursor(25,50);
  display.println(F("to turn back"));

  display.display();

  // Desbloquear el semáforo
  xSemaphoreGive(semaphoreDisplay);

  //Logica de si se llegara a pulsar cualquier boton
  while(!(  buttonState(PIN::Buttons::BACK ) == HIGH  ||
            buttonState(PIN::Buttons::UP   ) == HIGH  ||
            buttonState(PIN::Buttons::DOWN ) == HIGH  ||
            buttonState(PIN::Buttons::LEFT ) == HIGH  ||
            buttonState(PIN::Buttons::RIGHT) == HIGH  || 
            buttonState(PIN::Buttons::ENTER) == HIGH     ));
  delay(DEBOUNCE_TIME);  // delay para evitar pulsaciones de mas de las que el usuario no hizo voluntariamente

}

void Interface::EmergencyCalls::noSubProfileDeleted(void){

  // Bloquear el semáforo
  xSemaphoreTake( semaphoreDisplay, portMAX_DELAY );

  //Establezco los parametros a utilizar para la muestra a la salida del display
  display.clearDisplay();
  display.setTextColor(SH110X_WHITE);
  display.setTextSize(1);

  display.setCursor(30,10);
  display.println(F("Couldn't Succeded "));
  display.setCursor(10,20);
  display.println(F("Deleting The Sub-Profile"));

  //Le digo al usuario como terminar la ventana emergente
  display.setCursor(15,40);
  display.println(F("Press any button"));
  display.setCursor(25,50);
  display.println(F("to turn back"));

  display.display();
  
  // Desbloquear el semáforo
  xSemaphoreGive(semaphoreDisplay);

  //Logica de si se llegara a pulsar cualquier boton
  while(!(  buttonState(PIN::Buttons::BACK ) == HIGH  ||
            buttonState(PIN::Buttons::UP   ) == HIGH  ||
            buttonState(PIN::Buttons::DOWN ) == HIGH  ||
            buttonState(PIN::Buttons::LEFT ) == HIGH  ||
            buttonState(PIN::Buttons::RIGHT) == HIGH  || 
            buttonState(PIN::Buttons::ENTER) == HIGH     ));
  delay(DEBOUNCE_TIME);  // delay para evitar pulsaciones de mas de las que el usuario no hizo voluntariamente

}