#include "Profiles.h"

void Profiles::showProfiles_(void){

  //FUNCION DEPRECATED, despues ver su nueva funcion!!!!
  root.openRoot(volume);

  // list all files in the card with date and size
  root.ls(LS_R | LS_DATE | LS_SIZE);


}


void Profiles::createProfile_(const char* name){
  /*    Conversion de nombre recibido a tipo string archivo, (con el slash + name + extension)    */
  char* archiveString = strcat(SLASH_WITH_EOF_STR, name);
  archiveString = strcat(archiveString, extensionProfiles);

  SD.mkdir(archiveString) == true
    ? Serial.println("Successfully created")
    : Serial.println("Unsuccessfully created");

}

void Profiles::deleteProfile_(const char* name){

 
  char* archiveString = strcat(SLASH_WITH_EOF_STR, name); 
  archiveString = strcat(archiveString, extensionProfiles);

  //Elimina archivo
  SD.rmdir(archiveString) == true 
    ? Serial.println("Successfully eliminated")
    : Serial.println("Unsuccessfully eliminated");

}

void Profiles::createSubProfile_(const char* name, storedIRDataStruct* storedIRData){

  //Infrarred.read funcion esa para esperar el hexa y la cantidad de bits
  Cursor cursor();
  //cursor.write_ptr();
  //SD write y toda esa modiva para guardar el hexa, bits y infrarrojo
  // SD write agregar END al final

}