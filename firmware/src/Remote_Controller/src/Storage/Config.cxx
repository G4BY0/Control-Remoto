//Copyright Grupo 11, Inc. All Rights Reserved.
/***********************************************
 * * * * * * * * * * * * * * * * * * * * * * * *
 * 
 * Source of Modes/Clock.h
 *
 * * * * * * * * * * * * * * * * * * * * * * * *
***********************************************/
#include "Storage/Config.h"

IniFile config(CONFIG_FILE_NAME);

bool configSaved(void){

  //Primero doy prioridad a los settings de el almacenamiento externo
  if(configSaved_externStorage() == true)
    return true;

  //Luego, si falla uso los settings almacenados en la memoria flash
  return configSaved_internalStorage() == true  ? true  : false;

}

bool configSaved_internalStorage(void){

  Preferences preferences;

  // Abrir la memoria flash para retirar las preferencias
  if(preferences.begin("saved-config::services", true) == false){ // "saved-config::services" es el nombre del espacio de preferencias
  Serial.println(F("No config saved of services in flash memory."));
  return false;
  }

  WIFI_SERVICE_STATUS = preferences.getBool("service_WiFi" ,0);     //Recupero dato del servicio
  CLOCK_SERVICE_STATUS = preferences.getBool("service_CLOCK" ,0);    //Recupero dato del servicio
  BLUETOOTH_SERVICE_STATUS = preferences.getBool("service_BLUETOOTH" ,0);//Recupero dato del servicio

  // Abrir la memoria flash para retirar las preferencias
  if(preferences.begin("saved-config::clock", true)) // "saved-config::clock" es el nombre del espacio de preferencias
  return false;

  CLOCK_SERVICE_DIGITAL = preferences.getBool("digital" ,0);
  CLOCK_SERVICE_ANALOG = preferences.getBool("analog" ,0);

  preferences.end();

  return true;

}

bool configSaved_externStorage(void){
  
  constexpr size_t BUFFER_LENGTH = 1U;
  char buffer[BUFFER_LENGTH];

  if (!config.open()) {
    Serial.print(F("Ini file "));
    Serial.print(F(CONFIG_FILE_NAME " "));
    Serial.println(F("does not exist"));
    // Cannot do anything else
    return 0;
  }

  // Check the file is valid. This can be used to warn if any lines
  // are longer than the buffer.
  if (!config.validate(buffer, BUFFER_LENGTH)) {
    Serial.print(F("Ini file "));
    Serial.print(F(CONFIG_FILE_NAME " "));
    Serial.print(F("not valid: "));
    printErrorMessage(config.getError());
    // Cannot do anything else
    return 0;
  }


  #ifdef DEBUG
  
  // Fetch a value from a key which is present
  if (config.getValue("SERVICES", "CLOCK", buffer, BUFFER_LENGTH ,CLOCK_SERVICE_STATUS )) { // Key 'CLOCK' from section : SERVICES
    Serial.print(F("Section 'SERVICES' has an entry 'CLOCK' with value "));
    Serial.println(buffer);

  }
  else {
    Serial.print(F("Could not read 'CLOCK' from section 'SERVICES', error was ")); 
    printErrorMessage(config.getError());
  }
  
  // Try fetching a value from a missing key (but section is present)
  if (config.getValue("SERVICES", "WIFI", buffer, BUFFER_LENGTH ,WIFI_SERVICE_STATUS )) { // Key 'WIFI' from section : SERVICES
    Serial.print(F("Section 'SERVICES' has an entry 'nosuchkey' with value "));
    Serial.println(buffer);
  }
  else {
    Serial.print(F("Could not read 'BLUETOOTH' from section 'SERVICES', error was "));
    printErrorMessage(config.getError());
  }
  
  // Try fetching a key from a section which is not present
  if (config.getValue("SERVICES", "BLUETOOTH", buffer, BUFFER_LENGTH ,BLUETOOTH_SERVICE_STATUS )) { // Key 'BLUETOOTH' from section : SERVICES
    Serial.print(F("Section 'SERVICES' has an entry 'nosuchkey' with value "));
    Serial.println(buffer);
  }
  else {
    Serial.print(F("Could not read 'BLUETOOTH' from section 'SERVICES', error was "));
    printErrorMessage(config.getError());
  }

  //Fetching Keys from settings of clock
  // Try fetching a value from a missing key (but section is present)
  if (config.getValue("CLOCK", "DIGITAL", buffer, BUFFER_LENGTH ,CLOCK_DIGITAL_STATUS )) { // Key 'DIGITAL' from section : CLOCK
    Serial.print(F("Section 'SERVICES' has an entry 'nosuchkey' with value "));
    Serial.println(buffer);
  }
  else {
    Serial.print(F("Could not read 'BLUETOOTH' from section 'SERVICES', error was "));
    printErrorMessage(config.getError());
  }

  if (config.getValue("CLOCK", "ANALOG", buffer, BUFFER_LENGTH ,CLOCK_ANALOG_STATUS )) { // Key 'ANALOG' from section : CLOCK
    Serial.print(F("Section 'SERVICES' has an entry 'nosuchkey' with value "));
    Serial.println(buffer);
  }
  else {
    Serial.print(F("Could not read 'BLUETOOTH' from section 'SERVICES', error was "));
    printErrorMessage(config.getError());
  }

  return 1;

  #endif //DEBUG

}

void printErrorMessage(uint8_t e, bool eol)
{
  switch (e) {
  case IniFile::errorNoError:
    Serial.print(F("no error."));
    break;
  case IniFile::errorFileNotFound:
    Serial.print(F("file not found."));
    break;
  case IniFile::errorFileNotOpen:
    Serial.print(F("file not open."));
    break;
  case IniFile::errorBufferTooSmall:
    Serial.print(F("buffer too small."));
    break;
  case IniFile::errorSeekError:
    Serial.print(F("seek error."));
    break;
  case IniFile::errorSectionNotFound:
    Serial.print(F("section not found."));
    break;
  case IniFile::errorKeyNotFound:
    Serial.print(F("key not found."));
    break;
  case IniFile::errorEndOfFile:
    Serial.print(F("end of file."));
    break;
  case IniFile::errorUnknownError:
    Serial.print(F("unknown error."));
    break;
  default:
    Serial.print(F("unknown error value."));
    break;
  }
  if (eol)
    Serial.println();
}