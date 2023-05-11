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

}
//--------------------------------------------------------------------------------------------------------------------------------------------------

/*! Escribe en un buffer por linea, los archivos y directorios que se encuentran en raiz
    @returns buffer por linea
    @note En esta funcion, me fume alto porrazo y salio, pregunten cuando no este tan deserebrado porq la locura que hice no tiene sentido
*/
const char** Profiles::showProfiles_(void){
 
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
      
      if (archivo.isDirectory()){
        //Si es un directorio

      }else{
        //Si es un archivo
        Serial.print("Perfil: ");
        Serial.println(archivo.name());  //Imprimo el nombre

        profilesName = (char**) realloc(profilesName, sizeof( char* ) * (++numberOfProfiles) );
        profilesName[numberOfProfiles] = new char[sizeof(archivo.name())];
        profilesName[numberOfProfiles] = strcpy(profilesName[numberOfProfiles], archivo.name());

      }
      Serial.print("\n");
      
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

  }

  char* && profilePath = strcat(SLASH_WITH_EOF_STR, name); 
  profilePath = strcat(profilePath, extensionProfiles);

  
  //Elimina archivo
  SD.remove(profilePath) == true 
    ? Serial.println("Successfully eliminated.")
    : Serial.println("Unsuccessfully eliminated, check out if already doesn't exists...");

}

void SubProfiles::createSubProfile_(const char* subProfileName, storedIRDataStruct* storedIRData, const char* profileName){

  //Infrarred.read funcion esa para esperar el hexa y la cantidad de bits
  Cursor cursor();
  //cursor.write_ptr();
  //SD write y toda esa modiva para guardar el hexa, bits y infrarrojo
  

}

const char** SubProfiles::showSubProfiles(char* profileName){

  File rootForRead;
  File archivo;
  char** subProfilesName = nullptr;
  uint16_t numberOfSubProfiles = 0;
  
  /*    Conversion de nombre recibido a tipo string archivo, (con el slash + name + extension)    */
  profileName = strcat(SLASH_WITH_EOF_STR, profileName);
  profileName = strcat(profileName, extensionProfiles);

  archivo = (rootForRead.openNextFile());

  rootForRead = SD.open(profileName, FILE_READ);

  if(!rootForRead.available()){

    Serial.println("The file cannot be open successfully");
    return;

  }

  //Si es un archivo
  Serial.print("Perfil: ");
  Serial.println(archivo.name());  //Imprimo el nombre
  
  Serial.println("Subperfiles:");
  subProfilesName = (char**) realloc(subProfilesName, sizeof( char* ) * (++numberOfSubProfiles) );
  subProfilesName[numberOfSubProfiles] = new char[sizeof(archivo.name())];
  subProfilesName[numberOfSubProfiles] = strcpy(subProfilesName[numberOfSubProfiles], archivo.name());

  rootForRead.close();

}

Keep_t* ReturnSubProfile(const char* profileName, const char* subProfileName){

  File rootForRead;


  rootForRead = SD.open(profileName, FILE_READ);

  if(!rootForRead.available()){

    Serial.println("The file cannot be open successfully");
    return; //Failure

  }

  // Test de si corresponde lo que esta guardado. Sino, es porque hay otra cosa almacenada en vez de las estructuras
  if( ( sizeof(rootForRead.size()) % sizeof(Keep_t) ) != 0){ 

    Serial.println("The has wrong data stored, doesn't match with normalized information");
    return; //Failure

  }

  size_t && structPerFile = sizeof(rootForRead.size()) / sizeof(Keep_t);

  Keep_t* retiredFromSD = new ( Keep_t[structPerFile] ); 

  for( uint16_t iterator = 0 ; iterator < structPerFile; iterator++  ){

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

void SubProfiles::storeSubProfiles(Keep_t storeIR, const char* profileName){

  File rootForWrite;

  char* && profilePath = strcat(SLASH_WITH_EOF_STR, profileName); 
  profilePath = strcat(profilePath, extensionProfiles);

  rootForWrite = SD.open(profilePath,FILE_WRITE);

  if(!rootForWrite.available()){

    Serial.println("Unsuccessfull writting from File");
    return;

  }

  rootForWrite.write( (uint8_t*) &storeIR , sizeof(Keep_t) );
  
  rootForWrite.close();

}
