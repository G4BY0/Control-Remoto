//Copyright Grupo 11, Inc. All Rights Reserved.
/***********************************************
 * * * * * * * * * * * * * * * * * * * * * * * *
 * 
 * Source code of members of Profiles.h
 * 
 * * * * * * * * * * * * * * * * * * * * * * * *
***********************************************/

#include "Profiles.h"

String profilePath(const char* profileName) {
  //Convierto el string del dir
  String dirProfileName = PROFILES_DIR_PATH;
  dirProfileName += profileName;
  dirProfileName += EXTENSION_PROFILES;
  
  return dirProfileName;
}

void SDBegin(void){

  // Inicializar la comunicación con la tarjeta SD
  if (!SD.begin()) {
    Serial.println(F("Error al inicializar el almacenamiento."));
    Serial.println(F("Esperando respuesta del almacenamiento..."));
    while (!SD.begin());
  }

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

        //IGNORAR "Transfer.db" (archivo para el manejo del programa)
        if( strcmp( archivo.name() , TRANSFER_FILE_NAME ".db" ) == 0 ) continue; //Si son iguales...

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

bool Profiles::createProfile_(const char* name){
  
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

bool Profiles::deleteProfile_(const char* name){
  
  //Elimina archivo
  if( SD.remove(profilePath(name)) == true ){
    Serial.println(F("Successfully eliminated."));
    return true;
  } else {
    Serial.println(F("Unsuccessfully eliminated, check out if already doesn't exists..."));
    return false;
  }

}

void SubProfiles::createSubProfile_(const char* subProfileName, Protocols protocol, const char* profileName){

  //switch (protocol){

    //case Protocols::IR:
      auto IRData = storeCode( subProfileName );
      
      File root = SD.open( profilePath(profileName) , FILE_WRITE );

      //Si el archivo no esta disponible...
      if(!root){ 
        Serial.println(F("The file of the profile cannot be open successfully."));
        return; 
      }

      //Voy hasta el final del archivo
      root.seek(root.size());
      //Escritura de la informacion en el archivo
      root.write((const byte*) IRData.get(), sizeof(*IRData.get()) );
      root.flush();

      Serial.println(F("Successfull Uploaded SubProfile."));
      Serial.printf("Now the Profile weights %d\n" , root.size() );
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
    root.read(reinterpret_cast<byte*>(&Stored) , sizeof(Stored) ); // Leo en bytes la estructura para hacer el copiado
    Serial.println(Stored.nameSubProfile);                         
    subProfilesName.push_back(Stored.nameSubProfile);              // Guardo el String en el Vector Dinamico

  }

  root.close();

  return subProfilesName;
 
}

std::shared_ptr<storedIRDataStruct> SubProfiles::ReturnSubProfile(const char* profileName, const char* subProfileName){

  //Abro el archivo del perfil dado en modo Lectura
  File root = SD.open(profilePath(profileName), FILE_READ);

  //Si el archivo no esta disponible...
  if(!root){

    Serial.print(F("The file: "));
    Serial.print(profilePath(profileName));
    Serial.println(F("cannot be open successfully."));
    return nullptr; //Failure

  }
  
  std::shared_ptr<storedIRDataStruct> Stored(new storedIRDataStruct);
  
  
  root.seek(0U);
  while( root.position() != root.size() ){

    
    root.read(reinterpret_cast<byte*> (&( *Stored.get() ) ) , sizeof(& ( *Stored.get() ) ) );
    if( strcmp( Stored->nameSubProfile , subProfileName ) == 0 )
    break;

  }

  root.close();
  Serial.println(F("Successfull retired SubProfile Stored."));

  return Stored;

}

void SubProfiles::deleteSubProfile(const char* profileName, const char* subProfileName){

  //Abro los siguientes archivos para el trabajo de desplazamiento de informacion entre archivos
  File rootWriteOld = SD.open( profilePath(profileName) , FILE_READ);
  File rootReplacing = SD.open( TRANSFER_FILE_DIRANDNAME , FILE_WRITE);
  
  if(!rootWriteOld){
    Serial.println(F("Unsuccessfull writting from File."));
    return; //Failure, No se pudo abrir el archivo correctamente 
  }

  // Test de si corresponde lo que esta guardado. Sino, es porque hay otra cosa almacenada en vez de las estructuras
  if( ( sizeof(rootWriteOld.size()) % sizeof(storedIRDataStruct) ) != 0U ){ 
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

  for( uint32_t iterator = 0 ; iterator < structPerFile; iterator++ ){

    if(rootWriteOld.read( (uint8_t *) &retiredFromSD[iterator] , sizeof(storedIRDataStruct) ) != sizeof(storedIRDataStruct) ){

      Serial.print(F("Unsuccessfull reading from: "));
      Serial.println(profilePath(profileName));

    }
    else if(strcmp( retiredFromSD[iterator].nameSubProfile , subProfileName ) == EXIT_SUCCESS){ }
    else{ rootReplacing.write( (const byte*) &retiredFromSD[iterator] , sizeof(retiredFromSD[structPerFile]) );
    }

  }

  rootWriteOld.close();
  rootReplacing.close();
  
  rootWriteOld = SD.open(TRANSFER_FILE_DIRANDNAME , FILE_READ);
  rootReplacing = SD.open(profilePath(profileName) , FILE_WRITE);

  for( uint16_t iterator = 0 ; iterator < structPerFile; iterator++ ){

    if(rootWriteOld.read( (byte*) &retiredFromSD[iterator] , sizeof(storedIRDataStruct) ) != sizeof(storedIRDataStruct) ){

      Serial.print(F("Unsuccessfull reading from: "));
      Serial.println(TRANSFER_FILE_DIRANDNAME);
      Serial.print(F(" (Memoria reservada para el movimiento de informacion para la exterminacion del subperfil dado)"));

    }
    else if(strcmp( retiredFromSD[iterator].nameSubProfile , subProfileName ) == 0 ){ }
    else{  rootReplacing.write( (const byte*) &retiredFromSD[iterator] , sizeof(retiredFromSD[structPerFile]) ); }

  }

  delete[] retiredFromSD;
  rootReplacing.close();
  rootWriteOld.close();

}