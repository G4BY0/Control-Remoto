//Copyright Grupo 11, Inc. All Rights Reserved.
/***********************************************
 * * * * * * * * * * * * * * * * * * * * * * * *
 * 
 * Source code of members of Profiles.h
 * 
 * * * * * * * * * * * * * * * * * * * * * * * *
***********************************************/

#include "Storage/Profiles.h"

String profilePath(const char* profileName) {
  //Convierto el string del dir
  String dirProfileName = PROFILES_DIR_PATH;
  dirProfileName += profileName;
  dirProfileName += EXTENSION_PROFILES;
  
  return dirProfileName;
}

void SDBegin(void){

  // Configure all of the SPI select pins as outputs and make SPI
  // devices inactive, otherwise the earlier init routines may fail
  // for devices which have not yet been configured.
  pinMode(PIN::SD_t::chipSelect, OUTPUT);
  digitalWrite(PIN::SD_t::chipSelect, HIGH); // disable SD card

  // Inicializar la comunicación con la tarjeta SD
  if (!SD.begin()) {
    Serial.println(F("Failed to comunicate with external storage."));
    return;
  }

  // Archivo necesario para la necesidad de querer borrar subperfiles, espacio reservado para exclusivamente esa funcionalidad
  // Estaria bueno buscar otra forma de funcionamiento para optimizar el programa (bastantes recursos!)
  if(!SD.exists(TRANSFER_FILE_DIRANDNAME)){
  File transferFile = SD.open(TRANSFER_FILE_DIRANDNAME , FILE_WRITE);
  transferFile.close();
  }

}

std::vector<std::string> Profiles::getProfiles(void){
 
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

        //IGNORAR "Transfer.db" (archivo reservado para el manejo del programa)
        if( strcmp( archivo.name() , TRANSFER_FILE_NAME ".db" ) == 0 ) continue; //Si son iguales...
 
        //IGNORAR "config.ini" (archivo reservado para el manejo del programa)
        if( strcmp( archivo.name() , CONFIG_FILE_NAME ".ini" ) == 0 ) continue; //Si son iguales...

        //Si es un archivo 
        Serial.printf("Perfil: %s" ,archivo.name() );

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
  } while(archivo);
  
  archivo.close();
  root.close();
  return profilesName;

}

bool Profiles::createProfile(const char* name){
  
  File root = SD.open(profilePath(name), FILE_WRITE);
  if( root ){
    Serial.println(F("Successfully created."));
    return true;
  } else {
    Serial.println(F("Unsuccessfully created, check if already exists... \n If is that the case, first delete that profile."));
    return false;
  }
  root.close();

}

bool Profiles::deleteProfile(const char* name){
  
  //Elimina archivo
  if( SD.remove(profilePath(name)) == true ){
    Serial.println(F("Successfully eliminated."));
    return true;
  } else {
    Serial.println(F("Unsuccessfully eliminated, check out if already doesn't exists..."));
    return false;
  }

}

void SubProfiles::createSubProfile(const char* subProfileName, Protocols protocol, const char* profileName){

  //switch (protocol){

    //case Protocols::IR:
      auto IRData = storeCode( subProfileName );
      //(DEBUG)
      Serial.printf("Nombre de perfil a almacenar: %s\nNombre del subperfil a almacenar: %s" ,profileName ,subProfileName);


      File root = SD.open( profilePath(profileName) , FILE_WRITE );

      //Si el archivo no esta disponible...
      if(!root){ 
        Serial.println(F("The file of the profile cannot be open successfully."));
        return; 
      }

      //Voy hasta el final del archivo
      root.seek(root.size());
      //Escritura de la informacion en el archivo
      root.write((const byte*) IRData.get(), sizeof( *IRData.get() ) );
      root.flush();

      Serial.println(F("Successfull Uploaded SubProfile."));
      Serial.printf("Now the Profile weights %d Bytes\n" , root.size() );
      root.close();

    //break;

    //case Protocols::WIFI:
    //break;

    //case Protocols::BLUETOOTH:
    //break;

  //}

}

std::vector<std::string> SubProfiles::getSubProfiles(const char* profileName){

  std::vector<std::string> subProfilesName;
  uint16_t numberOfSubProfiles = 0U;

  File root = SD.open( profilePath(profileName) , FILE_READ );

  // Si el archivo no se abrio correctamente
  if(!root){

    Serial.println(F("The file cannot be open successfully."));
    return subProfilesName;

  }

  //Si es un archivo...
  Serial.printf("Perfil: %s\n" , root.name()); //Imprimo el nombre
  
  Serial.println(F("Subperfiles:"));
  root.seek(0U);
  // Comparo la posicion de bytes del puntero con el tamanio equivalente del archivo
  while( root.position() != root.size() ){
    storedIRDataStruct Stored;
    root.read(reinterpret_cast<byte*>(&Stored) , sizeof(Stored) );              // Leo en bytes la estructura para hacer el copiado
    subProfilesName.push_back(std::string(Stored.nameSubProfile));              // Guardo el String en el Vector Dinamico
    Serial.println(Stored.nameSubProfile);
  }

  root.close();

  return subProfilesName;
 
}

std::shared_ptr<storedIRDataStruct> SubProfiles::returnSubProfile(const char* profileName, const char* subProfileName){

  //Abro el archivo del perfil dado en modo Lectura
  File root = SD.open(profilePath(profileName), FILE_READ);

  //Si el archivo no esta disponible...
  if(!root){

    Serial.printf("The file: %s cannot be open successfully.\n" , profilePath(profileName) );
    return nullptr; //Failure

  }
  
  std::shared_ptr<storedIRDataStruct> Stored(new storedIRDataStruct);
  
  root.seek(0U);
  while( root.position() != root.size() ){

    root.read((byte*)  Stored.get()   , sizeof( ( *Stored.get() ) ));
    if( strcmp( Stored->nameSubProfile , subProfileName ) == 0 )
    break;

  }

  root.close();
  Serial.println(F("Successfull retired SubProfile Stored."));

  return Stored;

}

void SubProfiles::deleteSubProfile(const char* profileName, const char* subProfileName){

  //Abro los siguientes archivos para el trabajo de desplazamiento de informacion entre archivos
  File old_file = SD.open(  profilePath(profileName) , FILE_READ );
  File new_file = SD.open( TRANSFER_FILE_DIRANDNAME , FILE_WRITE);

  if(!old_file || !new_file){
    Serial.println(F("Unsuccessfull writting from File."));
    return; //Failure, No se pudo abrir el archivo correctamente 
  }

  storedIRDataStruct object_copy;
  while( old_file.position() != old_file.size() ){
    
    //Leo en mi buffer lo del fileStream
    old_file.read((byte*)  &object_copy   , sizeof(  object_copy  ));
    
    // Si no son iguales...
    if( strcmp( object_copy.nameSubProfile , subProfileName ) != 0 ) 
    new_file.write((const byte*)  &object_copy   , sizeof(  object_copy  ));
    
  }

  old_file.close();
  new_file.close();

}