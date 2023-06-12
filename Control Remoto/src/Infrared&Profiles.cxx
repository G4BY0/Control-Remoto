#include <IRremote.hpp>
#include "Infrared.h"
#include "Profiles.hpp"

#define DELAY_BETWEEN_REPEAT 50
#define DISABLE_LEDFEEDBACK false // false
#define ENABLE_LEDFEEDBACK true  // true

//Estructura de almacenamiento de datos del infrarrojo (sin el nombre del subperfil)
#ifndef storedIRDataStruct_type_definition
#define storedIRDataStruct_type_definition
struct storedIRDataStruct {
  IRData receivedIRData;
  // extensions for sendRaw
  uint8_t rawCode[RAW_BUFFER_LENGTH]; // The durations if raw
  uint8_t rawCodeLength; // The length of the code
};
#endif

//Estructura de almacenamiento de datos del infrarrojo (con el nombre del subperfil)
#ifndef Keep_type_definition
#define Keep_type_definition
struct Keep_t : public storedIRDataStruct{

  char nameSubProfile[20]; //Nombre del subperfil

};
#endif

void infraredBegin(void){

  pinMode(PIN::InfraredReceiver::DATA, INPUT);
  pinMode(PIN::InfraredTransmitter::DATA, OUTPUT);

  // Just to know which program is running on my Arduino
  Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRREMOTE));

  // Start the receiver and if not 3. parameter specified, take LED_BUILTIN pin from the internal boards definition as default feedback LED
  IrReceiver.begin(PIN::InfraredReceiver::DATA, DISABLE_LED_FEEDBACK, false);
  Serial.print(F("Ready to receive IR signals of protocols: "));
  printActiveIRProtocols(&Serial);
  
  Serial.print(F("at pin: "));
  Serial.println(PIN::InfraredReceiver::DATA);
  //IrSender.begin(); // Start with IR_SEND_PIN as send pin and enable feedback LED at default feedback LED pin
  //IrSender.begin(PIN::InfraredTransmitter::DATA); // Start with IR_SEND_PIN as send pin and enable feedback LED at default feedback LED pin
  IrSender.begin(PIN::InfraredTransmitter::DATA, DISABLE_LED_FEEDBACK, false); // si no funciona, chequear la de arriba
}

void Receive_start(void){

  // Restart receiver too
  Serial.println(F("Start Receiving for infrared signals"));
  IrReceiver.start();

}

bool Receive_check(void){

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
  if (IrReceiver.decode()) {
    
    return SUCCESS;
  }

  return FAILURE;
}

void Receive_stop(void){
  
  Serial.println(F("Stop receiving"));
  IrReceiver.resume(); //Resume receiver
  IrReceiver.stop();

}

void sendCode(Keep_t *aIRDataToSend) {

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
storedIRDataStruct* storeCode(void) {
  
  storedIRDataStruct* sStoredIRData = new storedIRDataStruct;

  //Copy decoded data
  sStoredIRData->receivedIRData = IrReceiver.decodedIRData;

  if (sStoredIRData->receivedIRData.protocol == UNKNOWN) {
    Serial.print(F("Received unknown code and store "));
    Serial.print(IrReceiver.decodedIRData.rawDataPtr->rawlen - 1);
    Serial.println(F(" timing entries as raw "));
    IrReceiver.printIRResultRawFormatted(&Serial, true); // Output the results in RAW format
    sStoredIRData->rawCodeLength = IrReceiver.decodedIRData.rawDataPtr->rawlen - 1;
    
    //Store the current raw data in a dedicated array for later usage
    IrReceiver.compensateAndStoreIRResultInArray(sStoredIRData->rawCode);
  } else {
    IrReceiver.printIRResultShort(&Serial);
    IrReceiver.printIRSendUsage(&Serial);
    sStoredIRData->receivedIRData.flags = 0; // clear flags -esp. repeat- for later sending
    Serial.println();
  }

  return sStoredIRData;

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//Copyright Grupo 7, Inc. All Rights Reserved.
/***********************************************
 * * * * * * * * * * * * * * * * * * * * * * * *
 * \file
 * Source code of members of Profiles.hpp
 * 
 * * * * * * * * * * * * * * * * * * * * * * * *
***********************************************/

//Estructura de almacenamiento de datos del infrarrojo (sin el nombre del subperfil)
#ifndef storedIRDataStruct_type_definition
#define storedIRDataStruct_type_definition
struct storedIRDataStruct {
  IRData receivedIRData;
  // extensions for sendRaw
  uint8_t rawCode[RAW_BUFFER_LENGTH]; // The durations if raw
  uint8_t rawCodeLength; // The length of the code
};
#endif

//Estructura de almacenamiento de datos del infrarrojo (con el nombre del subperfil)
#ifndef Keep_type_definition
#define Keep_type_definition
struct Keep_t : public storedIRDataStruct{

  char nameSubProfile[20]; //Nombre del subperfil

};
#endif

void SDBegin(void){

  #ifdef ARDUINOMEGA2560_CONFIGURATION
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
  #elif  defined(NODEMCUESP32S_CONFIGURATION)

  // Inicializar la comunicación con la tarjeta SD
  if (!SD.begin()) {
    Serial.println("Error al inicializar la tarjeta SD");
    while (!SD.begin());
  }

  #endif


  // Archivo necesario para la necesidad de querer borrar subperfiles, espacio reservado para exclusivamente esa funcionalidad
  // Estaria bueno buscar otra forma de funcionamiento para optimizar el programa (bastantes recursos!)
  if(!SD.exists(TRANSFER_FILE_DIRANDNAME)){
  File transferFile = SD.open(TRANSFER_FILE_DIRANDNAME);
  transferFile.close();
  }

}

char** Profiles::showProfiles_(void){
 
  File rootForRead;
  File archivo;

  char** profilesName = nullptr;
  uint16_t numberOfProfiles = 0;

  rootForRead = SD.open("/");
  do{
    archivo = (rootForRead.openNextFile());
    if(!archivo){
      //Si no hay archivo siguiente
      Serial.println(F("Doesn't find any other profile."));

    }else{

      if (!archivo.isDirectory()){
        //Si es un archivo 
        Serial.print(F("Perfil: "));
        Serial.println(archivo.name());  //Imprimo el nombre

        profilesName = (char**) realloc(profilesName, sizeof( char* ) * (++numberOfProfiles) );
        profilesName[numberOfProfiles] = new char[sizeof(archivo.name())];
        profilesName[numberOfProfiles] = strcpy(profilesName[numberOfProfiles], archivo.name());

      }

      Serial.print('\n');
      
    }
  }while(archivo);
  
  rootForRead.close();
  
  return profilesName;

}


void Profiles::createProfile_(const char* name){

  File rootForWrite;
  rootForWrite = SD.open(profilePath(name), FILE_WRITE);
  rootForWrite.available()
    ? Serial.println(F("Successfully created."))
    : Serial.println(F("Unsuccessfully created, check if already exists... \n If is that the case, first delete that profile."));
  rootForWrite.close();

}

void Profiles::deleteProfile_(const char* name){
  
  if(name == nullptr){
    
    Serial.println(F("nullptr Received, exit from deleProfile()"));
    return;

  }

  char* && profilePath = strcat(SLASH_WITH_EOF_STR, name); 
  profilePath = strcat( profilePath , extensionProfiles );

  //Elimina archivo
  SD.remove(profilePath) == true 
    ? Serial.println(F("Successfully eliminated."))
    : Serial.println(F("Unsuccessfully eliminated, check out if already doesn't exists..."));

}

Keep_t SubProfiles::convertIRData(storedIRDataStruct* storedIRData, const char* subProfileName){

  Keep_t storedIRDataWithStr;

  //Logica de copiado de una estructura a otra sumandole el nombre del subperfil
  storedIRDataWithStr.receivedIRData = storedIRData->receivedIRData; // Copio estructura de datos

  const size_t rawCodeSize = sizeof(storedIRData->receivedIRData);
  for( size_t iterator = 0 ; iterator < rawCodeSize ; (storedIRDataWithStr.receivedIRData = storedIRData->receivedIRData) )
    storedIRDataWithStr.rawCode[iterator] = storedIRData->rawCode[iterator];
  storedIRDataWithStr.rawCodeLength = storedIRData->rawCodeLength;

  strcpy( storedIRDataWithStr.nameSubProfile , subProfileName ); //Copio string, nombre del subperfil (char[20])

  delete[] storedIRData; // Lo borro porque no se usara mas
  return storedIRDataWithStr;
  
}

void SubProfiles::createSubProfile_(const char* subProfileName, storedIRDataStruct* storedIRData, const char* profileName){

  Keep_t Object = convertIRData(storedIRData, subProfileName);
  Keep_t* storedIRDataWithStr = &Object;

  File rootStoring;
  rootStoring = SD.open( profilePath(profileName) , FILE_WRITE );

  //Voy hasta el final del archivo
  rootStoring.seek(EOF);

  //Si el archivo no esta disponible...
  if(!rootStoring.available()){

    Serial.println(F("The file cannot be open successfully"));
    return; 

  }

  //Escritura de datos en el archivo
  rootStoring.write((uint8_t*) &storedIRDataWithStr, sizeof(storedIRDataWithStr) );

  rootStoring.close();
  
}

char** SubProfiles::showSubProfiles(const char* profileName){

  File rootRead;
  char** subProfilesName = nullptr;
  uint16_t numberOfSubProfiles = 0;

  rootRead = SD.open( profilePath(profileName) , FILE_READ );

  if(!rootRead.available()){

    Serial.println(F("The file cannot be open successfully"));
    return nullptr;

  }

  //Si es un archivo
  Serial.print(F("Perfil: "));
  Serial.println(rootRead.name());  //Imprimo el nombre
  
  Serial.println(F("Subperfiles:"));

  // Test de si corresponde lo que esta guardado. Sino, es porque hay otra cosa almacenada en vez de las estructuras
  if( ( sizeof(rootRead.size()) % sizeof(Keep_t) ) != 0){ 

    Serial.println(F("Profile has wrong data stored, doesn't match with normalized information"));
    return nullptr; //Failure

  }

  uint16_t && structPerFile = sizeof(rootRead.size()) / sizeof(Keep_t);

  Keep_t* retiredFromSD = new ( Keep_t[structPerFile] ); 

  for( uint16_t iterator = 0 ; iterator < structPerFile; iterator++ ){

    if(rootRead.read( (uint8_t *) (&retiredFromSD[iterator]) , sizeof(Keep_t) ) != sizeof(Keep_t) ){

      Serial.println(F("Unsuccessfull reading subProfile"));
      continue;

    }
    
    //Imprimo en el Serial el nombre del subperfil
    Serial.println(retiredFromSD[iterator].nameSubProfile);

    subProfilesName = (char**) realloc(subProfilesName, sizeof( char* ) * (iterator+1) );
    subProfilesName[iterator+1] = new char[sizeof(retiredFromSD[iterator].nameSubProfile)];
    strcpy( subProfilesName[iterator+1] , retiredFromSD[iterator].nameSubProfile );

  }

  rootRead.close();

}

Keep_t* SubProfiles::ReturnSubProfile(const char* profileName, const char* subProfileName){

  File rootForRead;

  rootForRead = SD.open(profilePath(profileName), FILE_READ);

  //Si el archivo no esta disponible...
  if(!rootForRead.available()){

    Serial.print(F("The file: "));
    Serial.print(profilePath(profileName));
    Serial.println(F("cannot be open successfully"));
    return nullptr; //Failure

  }

  // Test de si corresponde lo que esta guardado. Sino, es porque hay otra cosa almacenada en vez de las estructuras
  if( ( sizeof(rootForRead.size()) % sizeof(Keep_t) ) != 0){ 

    Serial.println(F("Profile has wrong data stored, doesn't match with normalized information"));
    return nullptr; //Failure

  }

  //Numero de subperfiles que se encuentran en el archivo
  uint16_t && structPerFile = sizeof(rootForRead.size()) / sizeof(Keep_t);

  Keep_t* retiredFromSD = new ( Keep_t[structPerFile] ); 

  for( uint16_t iterator = 0 ; iterator < structPerFile; iterator++ ){

    if(rootForRead.read( (uint8_t *) (&retiredFromSD[iterator]) , sizeof(Keep_t) ) != sizeof(Keep_t) ){
      
      Serial.println(F("Unsuccessfull reading from File"));

    }

    else if(strcmp( retiredFromSD[structPerFile].nameSubProfile , subProfileName ) == EXIT_SUCCESS){
      
      Keep_t IrStructFinded = retiredFromSD[structPerFile];
      delete[] retiredFromSD;
      return &IrStructFinded; //Success

    }

    return nullptr; // En caso de problemas

  }

  rootForRead.close();

}

void SubProfiles::storeSubProfile(Keep_t storeIR, const char* profileName){

  File rootForWrite;

  rootForWrite = SD.open( profilePath(profileName) , FILE_WRITE );

  //Voy hasta el final del archivo
  rootForWrite.seek(EOF);

  if(!rootForWrite.available()){

    Serial.println(F("Unsuccessfull opened Profile"));
    return;

  }

  rootForWrite.write( (uint8_t*) &storeIR , sizeof(Keep_t) );
  
  rootForWrite.close();

}

void SubProfiles::deleteSubProfile(const char* profileName, const char* subProfileName){

  File rootReplacing;
  File rootWriteOld;
 
  rootWriteOld = SD.open( profilePath(profileName) , FILE_READ);
  rootReplacing = SD.open( TRANSFER_FILE_DIRANDNAME , FILE_WRITE);
  
  if(!rootWriteOld.available()){

    Serial.println(F("Unsuccessfull writting from File"));
    return;

  }

  // Test de si corresponde lo que esta guardado. Sino, es porque hay otra cosa almacenada en vez de las estructuras
  if( ( sizeof(rootWriteOld.size()) % sizeof(Keep_t) ) != 0){ 

    Serial.println(F("Profile has wrong data stored, doesn't match with normalized information"));
    return; //Failure

  }

  size_t && structPerFile = sizeof(rootWriteOld.size()) / sizeof(Keep_t);

  Keep_t* retiredFromSD = new ( Keep_t[structPerFile] ); 

  for( uint16_t iterator = 0 ; iterator < structPerFile; iterator++ ){

    if(rootWriteOld.read( (uint8_t *) &retiredFromSD[iterator] , sizeof(Keep_t) ) != sizeof(Keep_t) ){

      Serial.print(F("Unsuccessfull reading from: "));
      Serial.println(profilePath(profileName));

    }
    else if(strcmp( retiredFromSD[structPerFile].nameSubProfile , subProfileName ) == EXIT_SUCCESS){ }
    else{

    rootReplacing.write( (uint8_t *) &retiredFromSD[structPerFile] , sizeof(retiredFromSD[structPerFile]) );

    }

  }

  rootWriteOld.close();
  rootReplacing.close();
  
  rootWriteOld = SD.open(TRANSFER_FILE_DIRANDNAME , FILE_READ);
  rootReplacing = SD.open(profilePath(profileName) , FILE_WRITE);

  for( uint16_t iterator = 0 ; iterator < structPerFile; iterator++ ){

    if(rootWriteOld.read( (uint8_t *) &retiredFromSD[iterator] , sizeof(Keep_t) ) != sizeof(Keep_t) ){

      Serial.print(F("Unsuccessfull reading from: "));
      Serial.println(TRANSFER_FILE_DIRANDNAME);

    }
    else if(strcmp( retiredFromSD[structPerFile].nameSubProfile , subProfileName ) == EXIT_SUCCESS){ }
    else{ 
      rootReplacing.write( (uint8_t *) &retiredFromSD[structPerFile] , sizeof(retiredFromSD[structPerFile]) ); 
    }

  }

  delete[] retiredFromSD;
  rootReplacing.close();
  rootWriteOld.close();

  

}