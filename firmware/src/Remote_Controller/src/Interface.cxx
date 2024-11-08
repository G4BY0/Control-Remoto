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
SemaphoreHandle_t semaphoreDisplay; //Semaforo para el libre uso del display

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
  xSemaphoreGive(semaphoreDisplay);

  display.begin(I2C_ADDRESS, true); //Inicializacion del display con la comunicacion I2C

  display.display(); //Expulso de la RAM hacia el display las instrucciones guardadas en el buffer (Adafruit_Industries->{LOGO})

  //Limpio la pantalla porque al inicio mete una imagen el buffer de entrada
  display.clearDisplay();
  display.display();

  //Establezco los parametros a utilizar para la muestra a la salida del display
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.display();

}

uint8_t Interface::hub(void){
  UI.show = true; //UI activado

  xSemaphoreTake( semaphoreDisplay , portMAX_DELAY ); // Bloquear el semáforo
  display.clearDisplay();
  display.setContrast(255); // Establecer el valor de contraste máximo
  display.display();
  xSemaphoreGive(semaphoreDisplay); // Desbloquear el semáforo

  //Opciones del menu Principal/Hub
  PROGMEM std::vector<std::string> strings  = { 
    "PROFILES", 
    "EDIT",
    "HELP", 
    "SHUTDOWN",
    "DIAGNOSTICS" 
  };

  //Cursor Para Hacer el manejo del Hub
  Cursor cursor( strings , display );
  int selected;

  do{ selected = cursor.getSelectedOption_number(); } 
  while(selected == -1);
  return selected;
}

void Interface::profiles(void){
  
  //Recibo Vector de Strings con el nombre de cada perfil cada uno (Ignorando el archivo reservado "Transfer.db")
  auto profiles_ptr = Profiles::getProfiles();

  if ( profiles_ptr.empty() == true) {
    Interface::EmergencyCalls::nonProfiles();
    return; // El puntero es nulo, salir de la función
  }

  Cursor cursor( profiles_ptr , display );
  const char* profile_selected = cursor.getSelectedOption();
  
  //Si el usuario no selecciono ninguna o hubo algun problema
  if(profile_selected == nullptr) return;

  //Muestro los subperfiles del perfil seleccionado por el usuario
  Interface::subProfiles( profile_selected );

}

void Interface::edit(void){

  PROGMEM std::vector<String> edit_options = {
    "ADD PROFILE",
    "DELETE PROFILE",
    "ADD SUBPROFILE",
    "DELETE SUBPROFILE"
  };

  Cursor cursor( edit_options , display );
  cursor.CutString = false;
  int edit_option_selected = cursor.getSelectedOption_number();

  switch (edit_option_selected) {
  default:
  case -1: return; //Si el usuario no selecciono ninguna o hubo algun problema
  case 0: Interface::addProfile();        break; // Crear perfil
  case 1: Interface::deleteProfile();     break; // Eliminar perfil
  case 2: Interface::addSubProfile();     break; // Agregar subperfil
  case 3: Interface::deleteSubProfile();  break; // Eliminar subperfil
  }

}

void Interface::addProfile(void){

  UI.show = false; //UI desactivado

  //Inicializo un Writter para recibir del usuario el nombre del nuevo perfil
  WritterV2 writter( &display );

  std::string profileName = writter.stringFinished();
  
  //En caso de cualquier problema incluyendo si el usuario cancelo la creacion de un nuevo perfil
  if( profileName.empty() ) return; //Failure, el usuario habia cancelado la escritura del nombre 

  //Creo el perfil en el almacenamiento con el nombre dado Y pregunto si no se cumplió correctamente...
  if(! Profiles::createProfile(profileName.c_str()) ) {
    Interface::EmergencyCalls::noProfileCreated();
    return;
  }

}

void Interface::deleteProfile(void){

  //Recibo del almacenamiento un vector de strings de los nombres de los perfiles
  auto names = Profiles::getProfiles();
  
  //Si recibo el vector vacio, hubo un problema
  if (names.empty()) {
    Interface::EmergencyCalls::nonProfiles();
    return; // El puntero es nulo, salir de la función
  }

  //Inicializo un Cursor para recibir del usuario el perfil seleccionado
  Cursor cursor(names,display);
  const char* selected = cursor.getSelectedOption();

  if(selected == nullptr) return;

  //Pido al almacenamiento eliminar el perfil dado
  Profiles::deleteProfile(selected);

}

void Interface::subProfiles(const char *profileName_){

  // Pido del almacenamiento los nombres de los subperfiles del perfil dado
  auto subprofiles = SubProfiles::getSubProfiles(profileName_); 

  if(subprofiles.empty()){
    Interface::EmergencyCalls::nonSubProfiles();
    ESP.restart();
  }

  // Inicializo Un cursor para pedir al usuario que subperfil desea seleccionar
  Cursor cursor( subprofiles ,display ); 

  for(const char* subprofile_selected = cursor.getSelectedOption() ; subprofile_selected != nullptr ; subprofile_selected = cursor.getSelectedOption()){
  
    if(strcmp(subprofile_selected, "") == 0) return; // Si no se selecciono ninguno...

    // Informacion a enviar a la salida
    auto IRToSend = SubProfiles::returnSubProfile( profileName_ , subprofile_selected ); //Pido del almacenamiento la informacion de la señal a transmitir del subperfil dado
    if( IRToSend == nullptr ){ // Si no se encuentra almacenada o hubo un error inesperado...
      Serial.println(F("The IR Signal can´t be send because has been received wrong IRDATA"));
      return;
    }

    sendCode(IRToSend); //Envio la señal a la salida con la informacion dada
  }

}

void Interface::addSubProfile(std::string profileSelected){

  if(profileSelected.empty()){

    auto namesProfile = Profiles::getProfiles(); // Recibo del almacenamiento todos los nombres de los perfiles

    //Si no llegara a recibir ningun nombre de ningun perfil o si hubo un error inesperado...
    if ( namesProfile.empty() ) {
      Interface::EmergencyCalls::nonProfiles();
      return; 
    }

    //Inicializo un cursor para preguntar en que perfil desea almacenar el nuevo subperfil
    Cursor cursor(namesProfile,display);
    const char* buffer = cursor.getSelectedOption();

    if(buffer == nullptr) return;
    profileSelected = buffer;

  }

  //Pido la informacion a la entrada
  if(!waitingForIR()) return; //Failure, Retorna que el usuario canceló la recepcion de la señal.

  UI.show = false; //UI desactivado

  //Inicializo un Writter para pedirle al usuario el nombre del nuevo subperfil
  WritterV2 writter(&display);
  std::string subProfileName = writter.stringFinished();

  if(subProfileName.empty()) return;

  // Bloquear el semáforo
  xSemaphoreTake( semaphoreDisplay , portMAX_DELAY );

  Serial.println(F("Storing... Please wait time."));
  display.setCursor( 20 , 20 );
  display.println(F("Storing..."));
  display.setCursor( 10 , 50 );
  display.print(F("Please wait time"));
  display.setCursor( 0 , 0 );

  // Desbloquear el semáforo
  xSemaphoreGive( semaphoreDisplay );

  //Creo en el almacenamiento el nuevo subperfil en el perfil junto a la informacion
  SubProfiles::createSubProfile(  subProfileName.c_str() , profileSelected.c_str() ); 

}

void Interface::deleteSubProfile(std::string profileSelected){

  //Si no recibo ningun nombre...
  if(profileSelected.empty()){

    //Recibo del almacenamiento el nombre de los perfiles
    auto namesProfiles = Profiles::getProfiles();

    if ( namesProfiles.empty() == true ) { //Si no recibo ningun nombre...
      Interface::EmergencyCalls::nonProfiles();
      return; // El puntero es nulo, salir de la función
    }

    //Inicializo un cursor para pedirle al usuario que perfil desea
    Cursor cursor( namesProfiles , display );
    const char* buffer = cursor.getSelectedOption();
    if(buffer == nullptr)
      return;
    profileSelected = buffer;

  }

  //Recido del almacenamiento el nombre de los subperfiles del perfil dado
  auto namesSubProfiles = SubProfiles::getSubProfiles(profileSelected.c_str());
  
  //Inicializo un cursor para pedirle al usuario que SUbperfil desea
  Cursor cursor2( namesSubProfiles , display ); 
  std::string selectedSubProfile = cursor2.getSelectedOption();

  //Si no recibo ningun Subperfil...
  if(selectedSubProfile.empty()) return; //Failure, el usuario cancelo la seleccion de subperfiles

  //Elimino el subperfil dado del subperfil dado
  SubProfiles::deleteSubProfile(profileSelected.c_str(), selectedSubProfile.c_str());

}

bool Interface::waitingForIR(void){

  UI.show = false;

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
      Serial.println(F("Canceled Receiving infrared signal."));
      delay(DEBOUNCE_TIME); //Rebote del fenomeno del pulsador
      return false; //Failure, se cancelo
    }
  }
  Receive_stop();
  Serial.println(F("End Receiving for infrared signals."));
  delay(DEBOUNCE_TIME); //Rebote del fenomeno del pulsador
  return true; //Recibido Correctamente

}

void Interface::battery(void){

  // Four tiles for a battery icon of 16x16.
  // The bitmap for the tiles is vertical!
  //    The first byte are the pixels on the left.
  //    The lowest bit is the pixel on top.
  PROGMEM const uint8_t batteryTile[] = 
  { 
    // The upper-left 8x8 pixels:
    0x00, 0x00, 0xF8, 0x0C, 0x04, 0x02, 0x02, 0x02, 
    // The upper-right 8x8 pixels:
    0x02, 0x02, 0x02, 0x04, 0x0C, 0xF8, 0x00, 0x00, 
    // The lower-left 8x8 pixels:
    0x00, 0x00, 0x7F, 0x40, 0x40, 0x40, 0x40, 0x40, 
    // The lower-right 8x8 pixels:
    0x40, 0x40, 0x40, 0x40, 0x40, 0x7F, 0x00, 0x00, 
  };

  const uint8_t && battery_percent = map( analogRead(PIN::Energy::BATTERY) , 0 , 4095 , 0 , 100 );

  uint8_t tiles[sizeof(batteryTile)];

  memcpy(tiles, batteryTile, 32);
  PROGMEM constexpr uint8_t  batX = 12;     // the 'x' (column) of the upper-left of the battery
  PROGMEM constexpr uint8_t batY = 3;     // the 'y' (row) of the upper-left of the battery

  // There are 12 lines to draw inside the battery.
  // Zero lines is also an option, so there are 13 possible levels.
  // There are 6 lines in the lower part and 6 lines in the upper part
  uint8_t lines = battery_percent / 8U; // might need some tuning near 0% and 100%
  lines = constrain( lines, 0, 12);     // for safety
  uint8_t lowerlines = min( (const uint8_t) uint8_t(6U), lines);      // 0...6 lines in the lower part
  uint8_t upperlines = 0U;
  if( lines > 6U)
  upperlines = lines - 6U;

  // The lines are over 4 bytes, and the tiles are vertical.
  // I can not think of any good code, so I just put down what it should do.

  // lower-left
  for( uint8_t i=3U; i<8U; i++)
  {
    for( uint8_t j=0U; j<lowerlines; j++)
      bitSet( tiles[i+16U], 7-(j+2U));
  }

  // lower-right
  for( uint8_t i=0U; i<5U; i++)
  {
    for( uint8_t j=0; j<lowerlines; j++)
      bitSet( tiles[i+24U], 7U-(j+2U));
  }

  // upper-left
  for( uint8_t i=3U; i<8U; i++)
  {
    for( uint8_t j=0; j<upperlines; j++)
      bitSet( tiles[i], 7U-j);
  }

  // upper-right
  for( uint8_t i=0U; i<5U; i++)
  {
    for( uint8_t j=0; j<upperlines; j++)
      bitSet( tiles[i+8U], 7U-j);
  }

  //display.drawTile( batX, batY,   2, tiles);                      // 50%-100%
  display.drawBitmap(batX, batY, tiles, 0, 8, SH110X_WHITE);        // 50%-100%
  //display.drawTile( batX, batY+1, 2, (uint8_t *) tiles + 16U);    // 50%-0%
  display.drawBitmap(batX, batY+1, tiles+16U, 0, 8, SH110X_WHITE);  // 50%-0%
  // Bloquear el semáforo
  xSemaphoreTake( semaphoreDisplay , portMAX_DELAY );

  // Desbloquear el semáforo
  xSemaphoreGive( semaphoreDisplay );

}

void Interface::clock( const struct tm& time , char* buff){

  PROGMEM const char* days[]= {
    "mon",
    "tue",
    "wed",
    "thu",
    "fri",
    "sat",
    "sun"
  };
  sprintf(buff, "%s %02i:%02i:%02i" ,days[time.tm_wday] ,time.tm_hour ,time.tm_min ,time.tm_sec); 
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
  display.setCursor( 15 , 40 );
  display.println(F("Press any button"));
  display.setCursor( 25 , 50 );
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