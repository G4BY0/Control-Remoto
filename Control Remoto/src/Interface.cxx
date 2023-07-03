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

  //Inicializacion del display con la comunicacion I2C
  display.begin(I2C_ADDRESS, true);

  //Expulso de la RAM hacia el display las instrucciones guardadas
  display.display();

  //Limpio la pantalla porque al inicio mete una imagen el buffer de entrada
  display.clearDisplay();
  display.display();

}

//DEBUGGING CURSOR
uint8_t Interface::hub(void){

  //Establezco los parametros a utilizar para la muestra a la salida del display
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  
  //Opciones del menu Principal/Hub
  std::vector<std::string> strings  = { "PROFILES" , "ADD PROFILE" , "DELETE PROFILE" , "ADD SUBPROFILE" , "DELETE SUBPROFILE" , "HELP" , "SLEEP" };


  Cursor cursor(strings,&display);
  const char* selected = cursor.getSelectedOption();
  //Si se cancelo la eleccion...
  if(selected == nullptr) return 255U;

  //Busco el numero de la opcion seleccionada y lo retorno
  for(uint8_t iterator = 0; iterator < strings.size(); iterator++ ){ 
    if(strcmp( selected , strings[iterator].c_str() ) == 0) 
    return iterator; 
  }

  return 255U; //Si hay problemas retorna

}
//---------------------------------------------------

void Interface::profiles(void){
  
  //Recibo Vector de Strings con el nombre de cada perfil cada uno (Ignorando el archivo reservado "Transfer.db")
  auto profiles_ptr = Profiles::showProfiles_();

  if ( profiles_ptr.empty() == true) {
    Interface::EmergencyCalls::nonProfiles();
    return; // El puntero es nulo, salir de la función
  }

  Cursor cursor( profiles_ptr , &display );
  const char* profile_selected = cursor.getSelectedOption();
  
  //Si el usuario no selecciono nada o hubo algun problema
  if(profile_selected == nullptr) return;

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

  //Pantalla Emergente que le pregunta al usuario si desea agregar un subperfil en este momento
  //Establezco los parametros a utilizar para la muestra a la salida del display
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);

  //Le digo al usuario por pantalla si desea agregar un Subperfil en este momento
  display.setCursor(5,10);
  display.println(F("Do you want to add \n a Subprofile Now?"));
  display.setCursor(10,30);
  display.println(F("Press Enter To Continue"));
  display.setCursor(10,40);
  display.println(F("Press any other to Cancel"));

  display.display();

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

  //Recbido del usuario (A traves del writter) el nombre del nuevo subperfil
  std::string subProfileName = writter.stringFinished(); //Recibo el nombre del subperfil seleccionado por el usuario
  if(subProfileName.c_str() == nullptr ) return; // SI no recibo ningun nombre...

  if(waitingForIR() == EXIT_FAILURE) return; //Si se cancela o se recibe un error al recibir la señal infrarroja (Se almacena en el objeto global "IrReceiver")
  yield();
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
  
  subprofiles.insert(  subprofiles.begin() , "ADD SUBPROFILE" );
  subprofiles.insert(  subprofiles.begin() , "DELETE SUBPROFILE" );

  /*
  // Si no hay ningun subperfil...
  if (subprofiles.empty() == true) {
    Interface::EmergencyCalls::nonSubProfiles();
    return; // El puntero es nulo, salir de la función
    
  }
  */

  // Inicializo Un cursor para pedir al usuario que subperfil desea seleccionar
  Cursor cursor( subprofiles ,&display); 
  const char* subprofiles_selected = cursor.getSelectedOption();
  if(subprofiles_selected == nullptr) return; // Si no se selecciono ninguno...

  if(subprofiles_selected == "ADD SUBPROFILE")
  Interface::addProfile();

  if(subprofiles_selected == "DELETE SUBPROFILE")
  Interface::deleteProfile();

  // Informacion a enviar a la salida
  auto IRToSend = SubProfiles::ReturnSubProfile( profileName_ , subprofiles_selected ); //Pido del almacenamiento la informacion de la señal a transmitir del subperfil dado

  if(IRToSend == nullptr ){ // Si no se encuentra almacenada o hubo un error inesperado...
    Serial.println(F("The IR Signal can´t be send because has been received wrong IRDATA"));
    return;
  }

  sendCode(IRToSend); //Envio la señal a la salida con la informacion dada 

}

void Interface::createSubProfile(void){

  auto namesProfile = Profiles::showProfiles_(); // Recibo del almacenamiento todos los nombres de los perfiles
  
  //Si no llegara a recibir ningun nombre de ningun perfil o si hubo un error inesperado...
  if ( namesProfile.empty() == true ) {
    Interface::EmergencyCalls::nonProfiles();
    return; 
  }


  //Inicializo un cursor para preguntar en que perfil desea almacenar el nuevo subperfil
  Cursor cursor(namesProfile,&display);
  const char* && profileSelected = cursor.getSelectedOption();

  //Pido la informacion a la entrada
  if(!waitingForIR()) return; //Failure, Retorna que el usuario canceló la recepcion de la señal.

  //Inicializo un Writter para pedirle al usuario el nombre del nuevo subperfil
  WritterV2 writter(&display);

  //Creo en el almacenamiento el nuevo subperfil en el perfil junto a la informacion
  SubProfiles::createSubProfile_(  writter.stringFinished() , Protocols::IR , profileSelected ); 
  
}

void Interface::deleteSubProfile(void){

  //Recibo del almacenamiento el nombre de los perfiles
  auto names = Profiles::showProfiles_();

  if ( names.empty() == true ) { //Si no recibo ningun nombre...
    Interface::EmergencyCalls::nonProfiles();
    return; // El puntero es nulo, salir de la función
  }

  //Inicializo un cursor para pedirle al usuario que perfil desea
  Cursor cursor( names , &display );
  const char* && selectedProfile = cursor.getSelectedOption();

  //Si no recibo ningun nombre...
  if(selectedProfile == nullptr) return; //Failure, el usuario cancelo la seleccion de perfiles

  //Recido del almacenamiento el nombre de los subperfiles del perfil dado
  names = SubProfiles::showSubProfiles(selectedProfile);
  
  //Inicializo un cursor para pedirle al usuario que SUbperfil desea
  Cursor cursor2( names , &display ); 
  const char* && selectedSubProfile = cursor2.getSelectedOption();

  //Si no recibo ningun Subperfil...
  if(selectedSubProfile == nullptr) return; //Failure, el usuario cancelo la seleccion de subperfiles

  //Elimino el subperfil dado del subperfil dado
  SubProfiles::deleteSubProfile(selectedProfile, selectedSubProfile);

}

bool Interface::waitingForIR(void){

  //Le hago saber al usuario por pantalla de que se esta esperando respuesta de la informacion

  //Establezco los parametros a utilizar para la muestra a la salida del display
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);

  display.setCursor(0,5);
  display.print(F("Prepared to Receive\nIR SIGNAL.\nWaiting For\nResponse... \nPress Any Botton\nTo Cancel."));
  display.display();

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
      delay(DEBOUNCE_TIME); //Rebote del fenomeno del pulsador
      return false; //Failure, se cancelo
      }
  }
  Receive_stop();
  delay(DEBOUNCE_TIME); //Rebote del fenomeno del pulsador
  return true; //Recibido Correctamente

}

void Interface::help(const char* text){

  //Establezco los parametros a utilizar para la muestra a la salida del display
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.display();
  
  QRCode qrcode;
  uint8_t qrcodeData[qrcode_getBufferSize(3)];
  qrcode_initText(&qrcode , qrcodeData , 3 , 0 , text );

  int scale = min( SCREEN_WIDTH / qrcode.size , SCREEN_HEIGHT / qrcode.size ); 

  int shiftX = ( SCREEN_WIDTH - qrcode.size * scale ) / 2;
  int shiftY = ( SCREEN_HEIGHT - qrcode.size * scale ) / 2;

  for(uint8_t y = 0 ; y < qrcode.size ; y++){
    for(uint8_t x = 0 ; x < qrcode.size ; x++){
      if(qrcode_getModule(&qrcode , x , y ))
        display.fillRect( shiftX + x * scale , shiftY + y * scale , scale , scale , SH110X_WHITE );
    }

  }
  display.display();


  //126 De alto y 51 de ancho el bitmap
  //display.drawBitmap( 0 , 0 , QRCode , 128 ,  64, SH110X_WHITE);
  //display.display();
  //A partir de X=0, Y=0
  

  //Si se presiona cualquier boton...
  while(!(buttonState(PIN::Buttons::BACK)  == HIGH  ||
          buttonState(PIN::Buttons::UP)    == HIGH  ||
          buttonState(PIN::Buttons::DOWN)  == HIGH  ||
          buttonState(PIN::Buttons::LEFT)  == HIGH  ||
          buttonState(PIN::Buttons::RIGHT) == HIGH  || 
          buttonState(PIN::Buttons::ENTER) == HIGH    ));
  delay(DEBOUNCE_TIME);  // DELAY PARA EL REBOTE DEL PULSADOR DE FENOMENO MECANICO  
}

void Interface::EmergencyCalls::nonProfiles(void){

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
  
  //Logica de si se llegara a pulsar cualquier boton
  while(!(  buttonState(PIN::Buttons::BACK ) == HIGH  ||
            buttonState(PIN::Buttons::UP   ) == HIGH  ||
            buttonState(PIN::Buttons::DOWN ) == HIGH  ||
            buttonState(PIN::Buttons::LEFT ) == HIGH  ||
            buttonState(PIN::Buttons::RIGHT) == HIGH  || 
            buttonState(PIN::Buttons::ENTER) == HIGH     ));
  delay(DEBOUNCE_TIME);  // delay para evitar pulsaciones de mas de las que el usuario no hizo voluntariamente

}