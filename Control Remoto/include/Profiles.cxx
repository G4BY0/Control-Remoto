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
String Profiles::showProfiles_(void){
 
  File rootForRead;
  File archivo;

  //Crear Cache.txt si es que no existe
  SD.mkdir("Cache.txt");
  
  //Cache abierto tipo escritura
  File cacheFile = SD.open("Cache.txt",FILE_WRITE);


  rootForRead = SD.open("/");
  int stringCounter = 0;
  String* profilesName;
  do{
    archivo = (rootForRead.openNextFile());
    if(!archivo){
        //Si no hay archivo siguiente
        Serial.println("END");
        cacheFile.println("END");

    }else{
      Serial.print(archivo.name());  //Imprimo el nombre
      if (archivo.isDirectory()){
          //Si es un directorio
          Serial.print("directorio");
          cacheFile.print("directorio");

          
      }else{
          //Si es un archivo
          Serial.print("archivo");
          cacheFile.print("archivo");
          profilesName = (String*) realloc(profilesName, sizeof(String) *  ++stringCounter );

      }
      Serial.print("\n");
      cacheFile.print("\n");
    }
  }while(archivo);

  rootForRead.close();
  cacheFile.close();

}


void Profiles::createProfile_(const char* name){
  /*    Conversion de nombre recibido a tipo string archivo, (con el slash + name + extension)    */
  char* directoryString = strcat(SLASH_WITH_EOF_STR, name);
  directoryString = strcat(directoryString, extensionProfiles);

  SD.mkdir(directoryString) == true
    ? Serial.println("Successfully created.")
    : Serial.println("Unsuccessfully created, check if already exists... \n If is the case, first delete that profile.");

}

void Profiles::deleteProfile_(const char* name){

 
  char* directoryString = strcat(SLASH_WITH_EOF_STR, name); 
  directoryString = strcat(directoryString, extensionProfiles);

  //Elimina archivo
  SD.rmdir(directoryString) == true 
    ? Serial.println("Successfully eliminated.")
    : Serial.println("Unsuccessfully eliminated, checkl if already doesn't exists...");

}

void Profiles::createSubProfile_(const char* name, storedIRDataStruct* storedIRData){

  //Infrarred.read funcion esa para esperar el hexa y la cantidad de bits
  Cursor cursor();
  //cursor.write_ptr();
  //SD write y toda esa modiva para guardar el hexa, bits y infrarrojo
  // SD write agregar END al final

}