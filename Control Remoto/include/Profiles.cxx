#include "Profiles.h"

/*! \deprecated Abria q extraer lo que se envia del serial osea leerlo, demasiado trabajo. Solucion abajo de esta
void Profiles::showProfiles_(void){

  //FUNCION DEPRECATED, despues ver su nueva funcion!!!!
  root.openRoot(volume);

  // list all files in the card with date and size
  root.ls(LS_R | LS_DATE | LS_SIZE);


}
*/

/*! Escribe en un buffer por linea, los archivos y directorios que se encuentran en raiz
    @returns buffer por linea
    @note En esta funcion, me fume alto porrazo y salio, pregunten cuando no este tan deserebrado porq la locura que hice no tiene sentido
*/
void Profiles::showProfiles_(void){
  /* EN DESARROLLO, PARA PROXIMAS VERSIONES SE USARA UN BUFFER EN VEZ DE UN ARCHIVO DE TEXTO COMO CACHE Y NO PILA
  //Depiendo del compilador BUFSIZ varia. Este va a ser el tama;o del buffer, con 256 basta.
  char* bufferOfStringsPerLine = new char[BUFSIZ];

  //Salida de mi buffer de strings
  __file StreamOfStringPerLine;
  FILE* StreamOfStringPerLine_ptr = &StreamOfStringPerLine;
  /*! -Seteo el buffer como entrada, 
      @param buffer de salida al que ira
      @param bufferOfstrings el buffer en cuestion a usar
      @param modeOfBuffer el tipo de modo de buffer, existen 3
      @param BUFSIZ tama;o del buffer en cuestion
      setvbuf( StreamOfStringPerLine_ptr , bufferOfStringsPerLine , _IOLBF , BUFSIZ );
  */
  
 
  File rootForRead;
  File archivo;

  SD.mkdir("Cache.txt");

  File cacheFile = SD.open("Cache.txt",FILE_WRITE);


  rootForRead = SD.open("/");

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
      }
      Serial.print("\n");
      cacheFile.print("\n");
    }
  }while(archivo);

  cacheFile.close();
  rootForRead.close();

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