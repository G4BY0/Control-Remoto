#include "Profiles.h"

// String extensionProfiles = ".txt"; <--- C++ WAY
const char* extensionProfiles = ".txt";
char* slash = "/";

void Profiles::showProfiles_(void){

  //FUNCION DEPRECATED, despues ver su nueva funcion!!!!
  root.openRoot(volume);

  // list all files in the card with date and size
  root.ls(LS_R | LS_DATE | LS_SIZE);


}

void Profiles::createProfile_(char* name){
  
  //String archive = '/' + name + extensionProfiles; <--- C++ WAY
  char* archiveString = strcat(slash, name); // <--- C WAY
  archiveString = strcat(archiveString, extensionProfiles);

  SD.mkdir(archiveString) == true
    ? Serial.println("Successfully created")
    : Serial.println("Unsuccessfully created");

}

void Profiles::deleteProfile_(char* name){

  //String archiveString = '/' + name + extensionProfiles; <--- C++ WAY
  char* archiveString = strcat(slash, name); // <--- C WAY
  archiveString = strcat(archiveString, extensionProfiles);

  //Elimina archivo
  SD.rmdir(archiveString) == true 
    ? Serial.println("Successfully eliminated")
    : Serial.println("Unsuccessfully eliminated");

}

void Profiles::createSubProfile_(char* name){

  //Infrarred.read funcion esa para esperar el hexa y la cantidad de bits
  Cursor cursor();
  //cursor.write_ptr();
  //SD write y toda esa modiva para guardar el hexa, bits y infrarrojo
  // SD write agregar END al final

}