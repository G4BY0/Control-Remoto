//Copyright Grupo 7, Inc. All Rights Reserved.
/***********************************************
 * * * * * * * * * * * * * * * * * * * * * * * *
 * \file
 * Source code of members of Profiles.hpp
 * 
 * * * * * * * * * * * * * * * * * * * * * * * *
***********************************************/

#include "Profiles.hpp"

void SDBegin(void){

  // CODIGO DE INICIALIZACION DE LIBRERIAS UTILES
  // TESTEANDO SI LA TARJETA SD ESTA TRABAJANDO!
  if (!card.init(SPI_HALF_SPEED, PIN::SD_t::chipSelect)) {
      Serial.println("initialization failed. Things to check:");
      Serial.println("* is a card inserted?");
      Serial.println("* is your wiring correct?");
      Serial.println("* did you change the chipSelect pin to match your shield or module?");
      while (!card.init(SPI_HALF_SPEED, PIN::SD_t::chipSelect));
  } else {
      Serial.println("Wiring is correct and a card is present.");
      SD.begin();
  }
  // TIPO DE TARJETA SD
  Serial.println();
  Serial.print("Card type:         ");
  switch (card.type()) {
    case SD_CARD_TYPE_SD1:
        Serial.println("SD1");
        break;
    case SD_CARD_TYPE_SD2:
        Serial.println("SD2");
        break;
    case SD_CARD_TYPE_SDHC:
        Serial.println("SDHC");
        break;
    default:
    Serial.println("Unknown");
  }

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
      Serial.println("Doesn't find any other profile.");
        
    }else{
      
      if (!archivo.isDirectory()){
        //Si es un archivo 
        Serial.print("Perfil: ");
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
  /*    Conversion de nombre recibido a tipo string archivo, (con el slash + name + extension)    */
  char* && profilePath = strcat(SLASH_WITH_EOF_STR, name);
  profilePath = strcat(profilePath, extensionProfiles);

  File rootForWrite;
  rootForWrite = SD.open(profilePath, FILE_WRITE);
  rootForWrite.available()
    ? Serial.println("Successfully created.")
    : Serial.println("Unsuccessfully created, check if already exists... \n If is that the case, first delete that profile.");
  rootForWrite.close();

}

void Profiles::deleteProfile_(const char* name){
  
  if(name == nullptr){

    Serial.println("nullptr Received, exit from deleProfile()");
    return;

  }

  char* && profilePath = strcat(SLASH_WITH_EOF_STR, name); 
  profilePath = strcat( profilePath , extensionProfiles );

  //Elimina archivo
  SD.remove(profilePath) == true 
    ? Serial.println("Successfully eliminated.")
    : Serial.println("Unsuccessfully eliminated, check out if already doesn't exists...");

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

  Keep_t* storedIRDataWithStr = (Keep_t*) &convertIRData(storedIRData, subProfileName);

  File rootStoring;
  rootStoring = SD.open( profilePath(profileName) , FILE_WRITE );

  //Si el archivo no esta disponible...
  if(!rootStoring.available()){

    Serial.println("The file cannot be open successfully");
    return; 

  }

  //Escritura de datos en el archivo
  rootStoring.write((uint8_t*) &storedIRDataWithStr, sizeof(storedIRDataWithStr) );

  rootStoring.close();
  
}

char** SubProfiles::showSubProfiles(char* profileName){

  File rootRead;
  char** subProfilesName = nullptr;
  uint16_t numberOfSubProfiles = 0;

  rootRead = SD.open( profilePath(profileName) , FILE_READ );

  if(!rootRead.available()){

    Serial.println("The file cannot be open successfully");
    return;

  }

  //Si es un archivo
  Serial.print("Perfil: ");
  Serial.println(rootRead.name());  //Imprimo el nombre
  
  Serial.println("Subperfiles:");

  // Test de si corresponde lo que esta guardado. Sino, es porque hay otra cosa almacenada en vez de las estructuras
  if( ( sizeof(rootRead.size()) % sizeof(Keep_t) ) != 0){ 

    Serial.println("Profile has wrong data stored, doesn't match with normalized information");
    return; //Failure

  }

  uint16_t && structPerFile = sizeof(rootRead.size()) / sizeof(Keep_t);

  Keep_t* retiredFromSD = new ( Keep_t[structPerFile] ); 

  for( uint16_t iterator = 0 ; iterator < structPerFile; iterator++ ){

    if(rootRead.read( (uint8_t *) (&retiredFromSD[iterator]) , sizeof(Keep_t) ) != sizeof(Keep_t) ){

      Serial.println("Unsuccessfull reading subProfile");
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

  rootForRead = SD.open(profileName, FILE_READ);

  //Si el archivo no esta disponible...
  if(!rootForRead.available()){

    Serial.println("The file cannot be open successfully");
    return; //Failure

  }

  // Test de si corresponde lo que esta guardado. Sino, es porque hay otra cosa almacenada en vez de las estructuras
  if( ( sizeof(rootForRead.size()) % sizeof(Keep_t) ) != 0){ 

    Serial.println("Profile has wrong data stored, doesn't match with normalized information");
    return; //Failure

  }

  //Numero de subperfiles que se encuentran en el archivo
  uint16_t && structPerFile = sizeof(rootForRead.size()) / sizeof(Keep_t);

  Keep_t* retiredFromSD = new ( Keep_t[structPerFile] ); 

  for( uint16_t iterator = 0 ; iterator < structPerFile; iterator++ ){

    if(rootForRead.read( (uint8_t *) (&retiredFromSD[iterator]) , sizeof(Keep_t) ) != sizeof(Keep_t) ){
      
      Serial.println("Unsuccessfull reading from File");

    }

    else if(strcmp( retiredFromSD[structPerFile].nameSubProfile , subProfileName ) == SUCCESS){
      
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

  if(!rootForWrite.available()){

    Serial.println("Unsuccessfull opened Profile");
    return;

  }

  rootForWrite.write( (uint8_t*) &storeIR , sizeof(Keep_t) );
  
  rootForWrite.close();

}

void SubProfiles::deleteSubProfile(const char* profileName, const char* subProfileName){

  File rootWriteNew;
  File rootWriteOld;
 
  rootWriteOld = SD.open( profilePath(profileName) );

  char * profileNameOld;
  strcpy(profileNameOld,profileName);
  strcpy(profileNameOld, "_old");

  
  if(!rootWriteOld.available()){

    Serial.println("Unsuccessfull writting from File");
    return;

  }

  // Test de si corresponde lo que esta guardado. Sino, es porque hay otra cosa almacenada en vez de las estructuras
  if( ( sizeof(rootWriteOld.size()) % sizeof(Keep_t) ) != 0){ 

    Serial.println("Profile has wrong data stored, doesn't match with normalized information");
    return; //Failure

  }

  size_t && structPerFile = sizeof(rootWriteOld.size()) / sizeof(Keep_t);

  Keep_t* retiredFromSD = new ( Keep_t[structPerFile] ); 

  for( uint16_t iterator = 0 ; iterator < structPerFile; iterator++ ){

    if(rootWriteOld.read( (uint8_t *) (&retiredFromSD[iterator]) , sizeof(Keep_t) ) != sizeof(Keep_t) ){

      Serial.println("Unsuccessfull reading from File");

    }
    else if(strcmp( retiredFromSD[structPerFile].nameSubProfile , subProfileName ) == SUCCESS){

      Keep_t IrStructFinded = retiredFromSD[structPerFile];
      delete[] retiredFromSD;
      return;
    }

  }

  rootWriteNew.close();
  rootWriteOld.close();

}