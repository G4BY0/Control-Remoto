//Copyright Grupo 11, Inc. All Rights Reserved.
/***********************************************
 * * * * * * * * * * * * * * * * * * * * * * * *
 * \file
 * (BUG) It is a fusion of 2 Files, Infrarred.cxx & Profiles.cxx. They are like this Because It causes a lot of errors when IRremote.hpp is linked...
 * 
 * * * * * * * * * * * * * * * * * * * * * * * *
***********************************************/
#include <IRremote.hpp>
#include "Infrared.h"
#include "Profiles.hpp"
//String extensionProfiles = ".db"; <--- C++ WAY
#define SLASH_WITH_EOF_STR "/"
#define extensionProfiles ".db"

/*! \brief Convierte a nombre de perfil a PATH (de manera normalizada, partiendo de raiz) usando los macros 'SLASH_WITH_EOF_STR' y 'extensionProfiles'
    \param string nombre de perfil a convertir en directorio
    \return puntero al string PATH*/
const char* profilePath(const char* profileName);
const char* profilePath(const char* profileName) {
  static std::string dirProfileName;
  dirProfileName.clear();
  dirProfileName.append(SLASH_WITH_EOF_STR); 
  dirProfileName.append(profileName);
  dirProfileName.append(extensionProfiles);
  return dirProfileName.c_str();
}
/***********************************************
 * * * * * * * * * * * * * * * * * * * * * * * *
 * 
 * Source code of members of Infrared.h
 * 
 * * * * * * * * * * * * * * * * * * * * * * * *
***********************************************/

/* (BUG) ELIMINADO DEL CODIGO PORQUE DABA ERRORES CON LOS TIMERS, NOSE PORQUE PERO CALCULO QUE SERA EL QUE HABLA DE LEDFEEDBACK
#define DELAY_BETWEEN_REPEAT 50
#define DISABLE_LEDFEEDBACK false // false
#define ENABLE_LEDFEEDBACK true  // true
*/

//Estructura de almacenamiento de datos del infrarrojo (sin el nombre del subperfil)
struct storedIRDataStruct {
  IRData receivedIRData;
  // extensions for sendRaw
  uint8_t rawCode[RAW_BUFFER_LENGTH]; // The durations if raw
  uint8_t rawCodeLength; // The length of the code

  char nameSubProfile[30]; //Nombre del subperfil
};

void infraredBegin(void){

  pinMode(PIN::InfraredReceiver::DATA, INPUT);
  pinMode(PIN::InfraredTransmitter::DATA, OUTPUT);

  // Just to know which program is running on my Arduino
  Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRREMOTE));

  // Inicializacion del IRReceiver
  IrReceiver.begin(PIN::InfraredReceiver::DATA, DISABLE_LED_FEEDBACK, false);
  Serial.print(F("Ready to receive IR signals of protocols: "));
  printActiveIRProtocols(&Serial);
  
  // Inicializacion del IRSender
  Serial.print(F("at pin: "));
  Serial.println(PIN::InfraredReceiver::DATA);
  IrSender.begin(PIN::InfraredTransmitter::DATA, DISABLE_LED_FEEDBACK, false); // si no funciona, chequear la de arriba
  
}

void Receive_start(void){
  IrReceiver.start();
  IrReceiver.resume(); //Resume receiver
}

bool Receive_check(void){
  //Logica de checkeo si la informacion recibida es correcta o incorrecta...
  if( IrReceiver.decode() ){
    if (IrReceiver.decodedIRData.rawDataPtr->rawlen < 4) {
      Serial.print(F("Ignore data with rawlen="));
      Serial.println(IrReceiver.decodedIRData.rawDataPtr->rawlen);
      return FAILURE;
     }
    if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT) {
      Serial.println(F("Ignore repeat"));
      return FAILURE;
    }
    if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_AUTO_REPEAT) {
      Serial.println(F("Ignore autorepeat"));
      return FAILURE;
    }
    if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_PARITY_FAILED) {
      Serial.println(F("Ignore parity error"));
      return FAILURE;
    }
    
    return SUCCESS; 
  }
  return FAILURE;
}

void Receive_stop(void){
  
  Serial.println(F("Stop receiving"));
  IrReceiver.stop(); //Libero la carga al procesador que produce la lectura de infrarrojos.

}

void sendCode(std::shared_ptr<storedIRDataStruct> aIRDataToSend) {
  Serial.flush(); // To avoid disturbing the software PWM generation by serial output interrupts
  if (aIRDataToSend->receivedIRData.protocol == UNKNOWN /* i.e. raw */) {
    // Assume 38 KHz
    IrSender.sendRaw(aIRDataToSend->rawCode, aIRDataToSend->rawCodeLength, 38);

    Serial.print(F("raw "));
    Serial.print(aIRDataToSend->rawCodeLength);
    Serial.println(F(" marks or spaces"));
  } else {
    // Use the write function, which does the switch for different protocols
    IrSender.write(&aIRDataToSend->receivedIRData);
    printIRResultShort(&Serial, &aIRDataToSend->receivedIRData, false);
  }
}

// Stores the code for later playback
storedIRDataStruct storeCode(const char* subprofileName) {

  storedIRDataStruct sStoredIRData;

  //Copy decoded data
  sStoredIRData.receivedIRData = IrReceiver.decodedIRData;

  if (sStoredIRData.receivedIRData.protocol == UNKNOWN) {
    Serial.print(F("Received unknown code and store "));
    Serial.print(IrReceiver.decodedIRData.rawDataPtr->rawlen - 1);
    Serial.println(F(" timing entries as raw "));
    IrReceiver.printIRResultRawFormatted(&Serial, true); // Output the results in RAW format
    sStoredIRData.rawCodeLength = IrReceiver.decodedIRData.rawDataPtr->rawlen - 1;
    
    //Store the current raw data in a dedicated array for later usage
    IrReceiver.compensateAndStoreIRResultInArray(sStoredIRData.rawCode);
  } else {
    IrReceiver.printIRResultShort(&Serial);
    IrReceiver.printIRSendUsage(&Serial);
    sStoredIRData.receivedIRData.flags = 0; // clear flags -esp. repeat- for later sending
    Serial.print( '\n' );
  }

  return sStoredIRData;

}

//Copyright Grupo 11, Inc. All Rights Reserved.
/***********************************************
 * * * * * * * * * * * * * * * * * * * * * * * *
 * 
 * Source code of members of Infrared.h
 * 
 * * * * * * * * * * * * * * * * * * * * * * * *
***********************************************/

//DEBUGGING!!!!!!!!!!!!!!!!!!!!!
void julio(){
  Receive_start();
  //Pido la informacion a la entrada
  while( [&]() -> bool {
    
    while( Receive_check() ){
        //Logica de si se llegara a presionar algun boton
    if(  !digitalRead(PIN::Buttons::BACK)   ||
          !digitalRead(PIN::Buttons::UP)     ||
          !digitalRead(PIN::Buttons::DOWN)   ||
          !digitalRead(PIN::Buttons::LEFT)   ||
          !digitalRead(PIN::Buttons::RIGHT)  ||
          !digitalRead(PIN::Buttons::ENTER)    ) {
        Receive_stop();
        delay(500); //Rebote del fenomeno del pulsador
        return EXIT_FAILURE; //Failure, se cancelo
        }
    }
    Receive_stop();
    delay(500); //Rebote del fenomeno del pulsador
    return EXIT_SUCCESS; //Failure, se cancelo

  }()); //Failure, Retorna que el usuario canceló la recepcion de la señal.

  std::shared_ptr<storedIRDataStruct> pepe (new storedIRDataStruct);
  storedIRDataStruct kgkg = storeCode("Julian");
  memcpy(pepe.get() , &kgkg , sizeof(storedIRDataStruct) );
  sendCode(pepe);
  Serial.println("Code Sended");

}

//--------------------------------

void SDBegin(void){

  #if defined(ESP32) || defined(ESP8266)

  // Inicializar la comunicación con la tarjeta SD
  if (!SD.begin()) {
    Serial.println(F("Error al inicializar el almacenamiento"));
    Serial.println(F("Esperando respuesta del almacenamiento..."));
    while (!SD.begin());
  }

  #elif defined(__AVR__)
  Sd2Card card;
  // CODIGO DE INICIALIZACION DE LIBRERIAS UTILES
  // TESTEANDO SI LA TARJETA SD ESTA TRABAJANDO!
  if (!card.init(SPI_HALF_SPEED, PIN::SD_t::chipSelect)) {
      Serial.println(F("initialization failed. Things to check:"));
      Serial.println(F("* is a card inserted?"));
      Serial.println(F("* is your wiring correct?"));
      Serial.println(F("* did you change the chipSelect pin to match your shield or module?"));
      while (!card.init(SPI_HALF_SPEED, PIN::SD_t::chipSelect));
  } else {
      Serial.println(F("Wiring is correct and a card is present."));
      SD.begin();
  }
  // TIPO DE TARJETA SD
  Serial.println();
  Serial.print(F("Card type:         "));
  switch (card.type()) {
    case SD_CARD_TYPE_SD1:
        Serial.println(F("SD1"));
        break;
    case SD_CARD_TYPE_SD2:
        Serial.println(F("SD2"));
        break;
    case SD_CARD_TYPE_SDHC:
        Serial.println(F("SDHC"));
        break;
    default:
    Serial.println(F("Unknown"));
  }

  #endif

  // Archivo necesario para la necesidad de querer borrar subperfiles, espacio reservado para exclusivamente esa funcionalidad
  // Estaria bueno buscar otra forma de funcionamiento para optimizar el programa (bastantes recursos!)
  if(!SD.exists(TRANSFER_FILE_DIRANDNAME)){
  File transferFile = SD.open(TRANSFER_FILE_DIRANDNAME , FILE_WRITE);
  transferFile.close();
  }

}

std::vector<std::string> Profiles::showProfiles_(void){
 
  std::vector<std::string> profilesName;

  File root = SD.open("/"); // Abro como Root 
  File archivo; //Flujo para el movimiento entre los archivos
  do{
    archivo = (root.openNextFile());
    if(!archivo){
      //Si no hay archivo siguiente
      Serial.println(F("Doesn't find any other profile."));

    }else{

      if (!archivo.isDirectory()){

        //Condicion EXCLUSIVA para IGNORAR "Transfer.db" (archivo para el manejo del programa)
        if( strcmp( archivo.name() , "Transfer.db" ) == 0 ) continue; //Si son iguales...

        //Si es un archivo 
        Serial.print(F("Perfil: "));
        Serial.println(F(archivo.name()));  //Imprimo el nombre

        profilesName.push_back( [=] () -> std::string {
          std::string name = archivo.name();
          const size_t position = name.find( '.' );
          
          if (position != std::string::npos) { // std::string::npos es devuelto por el metodo find() si es que no se llegara a encontrar el caracter dado
          return name.substr(0U, position); // Retorna la cadena hasta '.'
          }
          return name;

        }() ); //Guardo el nombre en el vector sin la extension

      }

      Serial.print('\n');
      
    }
  }while(archivo);
  
  archivo.close();
  root.close();
  return profilesName;

}


void Profiles::createProfile_(const char* name){
  
  File root = SD.open(profilePath(name), FILE_WRITE);
  root.available()
    ? Serial.println(F("Successfully created."))
    : Serial.println(F("Unsuccessfully created, check if already exists... \n If is that the case, first delete that profile."));
  root.close();

}

void Profiles::deleteProfile_(const char* name){
  
  //Elimina archivo
  SD.remove(profilePath(name)) == true 
    ? Serial.println(F("Successfully eliminated."))
    : Serial.println(F("Unsuccessfully eliminated, check out if already doesn't exists..."));

}

void SubProfiles::createSubProfile_(const char* subProfileName, Protocols protocol, const char* profileName){

  //switch (protocol){
    
    //case Protocols::IR:
      auto IRData = storeCode(subProfileName);

      File root = SD.open( profilePath(profileName) , FILE_WRITE );

      //Si el archivo no esta disponible...
      if(!root.available()){
        Serial.println(F("The file of the profile cannot be open successfully"));
        return; 
      }

      //Voy hasta el final del archivo
      root.seek(EOF);
      //Escritura de la informacion en el archivo
      root.write((uint8_t*) &IRData, sizeof(IRData) );
      root.close();

    //break;

    //case Protocols::WIFI:
    //break;

    //case Protocols::BLUETOOTH:
    //break;


  //}

}

std::vector<std::string> SubProfiles::showSubProfiles(const char* profileName){

  std::vector<std::string> subProfilesName;
  uint16_t numberOfSubProfiles = 0U;

  File root = SD.open( profilePath(profileName) , FILE_READ );

  if(!root.available()){

    Serial.println(F("The file cannot be open successfully"));
    return subProfilesName;

  }

  //Si es un archivo...
  Serial.println(F("Perfil: "));
  Serial.println(F(root.name()));  //Imprimo el nombre
  
  Serial.println(F("Subperfiles:"));
  // Test de si corresponde lo que esta guardado. Sino, es porque hay otra cosa almacenada en vez de las estructuras o hubo un error inesperado
  if( ( sizeof(root.size()) % sizeof(storedIRDataStruct) ) != 0U){ 

    Serial.println(F("Profile has wrong data stored, doesn't match with normalized information"));
    return subProfilesName; //Failure

  }

  //Cantidad de perfiles que se encuentran almacenados
  const size_t && structPerFile = sizeof(root.size()) / sizeof(storedIRDataStruct);

  //Reservo memoria para el movimiento de la informacion
  storedIRDataStruct* retiredData = new storedIRDataStruct[structPerFile]; 

  for( uint16_t iterator = 0U ; iterator < structPerFile; iterator++ ){
    //Luego eliminar este If cuando se permita que el nombre del subperfil sea variable. Esto puede causar problemas...
    if(root.read( (uint8_t *) (&retiredData[iterator]) , sizeof(storedIRDataStruct) ) != sizeof(storedIRDataStruct) ){
      Serial.println(F("Unsuccessfull reading subProfile"));
      continue;
    }
    
    //Imprimo en el Serial el nombre del subperfil
    Serial.print(F("Nombre del subperfil extraido del almacenamiento: "));
    Serial.println(retiredData[iterator].nameSubProfile);
    subProfilesName.push_back(retiredData[iterator].nameSubProfile);

  }

  delete[] retiredData;

  root.close();

  return subProfilesName;
 
}

std::shared_ptr<storedIRDataStruct> SubProfiles::ReturnSubProfile(const char* profileName, const char* subProfileName){

  //Abro el archivo del perfil dado en modo Lectura
  File root = SD.open(profilePath(profileName), FILE_READ);

  //Si el archivo no esta disponible...
  if(!root.available()){

    Serial.print(F("The file: "));
    Serial.print(profilePath(profileName));
    Serial.println(F("cannot be open successfully"));
    return nullptr; //Failure

  }

  // Test de si corresponde lo que esta guardado. Sino, es porque hay otra cosa almacenada en vez de las estructuras
  if( ( sizeof(root.size()) % sizeof(storedIRDataStruct) ) != 0){ 

    Serial.println(F("Profile has wrong data stored, doesn't match with normalized information"));
    return nullptr; //Failure

  }

  //Numero de subperfiles que se encuentran en el archivo
  const size_t && structPerFile = sizeof(root.size()) / sizeof(storedIRDataStruct);

  //Reservo memoria para la inforamacion retirada del almacenamiento
  std::shared_ptr<storedIRDataStruct[]> retiredFromSD ( new storedIRDataStruct[structPerFile] );

  for( uint16_t iterator = 0U ; iterator < structPerFile; iterator++ ){

    if(root.read( (uint8_t *) (&retiredFromSD[iterator]) , sizeof(storedIRDataStruct) ) != sizeof(storedIRDataStruct) ){
      
      Serial.println(F("Unsuccessfull reading from File"));

    }

    else if(strcmp( retiredFromSD[iterator].nameSubProfile , subProfileName ) == EXIT_SUCCESS){
      
      std::shared_ptr<storedIRDataStruct> catchIt (&retiredFromSD[iterator]);
      
      return catchIt; //Success
    }

    return nullptr; 
    /*En caso de problemas, si hay algo que no va bien retorna "nullptr" para evitar errores futuros en caso de seguir leyendo el almacenamiento
     *Haciendo eso evito problemas que podria generar el bajo nivel de energia de suplementacion de el almacenamiento
    */

  }

  root.close();

}

void SubProfiles::storeSubProfile(std::shared_ptr<storedIRDataStruct> storeIR, const char* profileName){

  //Abro el archivo del perfil a almacenar el subperfil dado (abierto en modo escritura)
  File root = SD.open( profilePath(profileName) , FILE_WRITE );

  //Si el flujo del archivo no esta disponible...
  if(!root.available()){
    Serial.println(F("Unsuccessfull opened Profile"));
    return; //Failure, No se pudo abrir el archivo correctamente
  }

  //Voy hasta el final del archivo
  root.seek(EOF);

  //Escribo en el perfil la informacion dada
  root.write( (uint8_t*) storeIR.get() , sizeof(storedIRDataStruct) ); 
  
  root.close();

}

void SubProfiles::deleteSubProfile(const char* profileName, const char* subProfileName){

  //Abro los siguientes archivos para el trabajo de desplazamiento de informacion entre archivos
  File rootWriteOld = SD.open( profilePath(profileName) , FILE_READ);
  File rootReplacing = SD.open( TRANSFER_FILE_DIRANDNAME , FILE_WRITE);
  
  if(!rootWriteOld.available()){
    Serial.println(F("Unsuccessfull writting from File."));
    return;//Failure, No se pudo abrir el archivo correctamente 
  }

  // Test de si corresponde lo que esta guardado. Sino, es porque hay otra cosa almacenada en vez de las estructuras
  if( ( sizeof(rootWriteOld.size()) % sizeof(storedIRDataStruct) ) != 0){ 
    Serial.println(F("Profile has wrong data stored, doesn't match with normalized information."));
    return;
    /*En caso de problemas, si hay algo que no va bien retorna "nullptr" para evitar errores futuros en caso de seguir leyendo el almacenamiento
     *Haciendo eso evito problemas que podria generar el bajo nivel de energia de suplementacion de el almacenamiento
    */
  }

  //Numero total de subperfiles almacenados
  const size_t structPerFile = sizeof( rootWriteOld.size() ) / sizeof(storedIRDataStruct);

  //Reservo Memoria para buscar la informacion a eliminar
  storedIRDataStruct* retiredFromSD = new storedIRDataStruct[structPerFile]; 

  for( uint16_t iterator = 0 ; iterator < structPerFile; iterator++ ){

    if(rootWriteOld.read( (uint8_t *) &retiredFromSD[iterator] , sizeof(storedIRDataStruct) ) != sizeof(storedIRDataStruct) ){

      Serial.print(F("Unsuccessfull reading from: "));
      Serial.println(profilePath(profileName));

    }
    else if(strcmp( retiredFromSD[iterator].nameSubProfile , subProfileName ) == EXIT_SUCCESS){ }
    else{ rootReplacing.write( (uint8_t *) &retiredFromSD[iterator] , sizeof(retiredFromSD[structPerFile]) );
    }

  }

  rootWriteOld.close();
  rootReplacing.close();
  
  rootWriteOld = SD.open(TRANSFER_FILE_DIRANDNAME , FILE_READ);
  rootReplacing = SD.open(profilePath(profileName) , FILE_WRITE);

  for( uint16_t iterator = 0 ; iterator < structPerFile; iterator++ ){

    if(rootWriteOld.read( (uint8_t *) &retiredFromSD[iterator] , sizeof(storedIRDataStruct) ) != sizeof(storedIRDataStruct) ){

      Serial.print(F("Unsuccessfull reading from: "));
      Serial.println(TRANSFER_FILE_DIRANDNAME);
      Serial.print(F(" (Memoria reservada para el movimiento de informacion para la exterminacion del subperfil dado)"));

    }
    else if(strcmp( retiredFromSD[iterator].nameSubProfile , subProfileName ) == 0 ){ }
    else{  rootReplacing.write( (uint8_t *) &retiredFromSD[iterator] , sizeof(retiredFromSD[structPerFile]) ); }

  }

  delete[] retiredFromSD;
  rootReplacing.close();
  rootWriteOld.close();

}