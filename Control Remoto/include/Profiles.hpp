//Copyright Grupo 7, Inc. All Rights Reserved.
/***********************************************
 * * * * * * * * * * * * * * * * * * * * * * * *
 * \file
 * Movimento de almacenamiento
 * de perfiles dentro de la SD
 * usando la PILA para su desplazamiento
 * 
 * * * * * * * * * * * * * * * * * * * * * * * *
***********************************************/

#ifndef PROFILES_H
#define PROFILES_H

#include "PIN.h"
#include <SD.h>
#include "Interface.h" // INCHEQUEABLE, DESPUES AVERIGUAR
#include "Infrared.h"
#define TRANSFER_FILE_DIRANDNAME "/Transfer.db"

//Polimorfismo de la estructura de la DATA del infrarrojo que implementa el nombre dado por el usuario
struct Keep_t : public storedIRDataStruct {

    char nameSubProfile[20];

};

//Dependiendo del sistema operativo, incluye o no la definicion.
#ifndef _IOFBF
#define _IOFBF            0x0000
#endif
#ifndef _IOLBF
#define _IOLBF            0x0040
#endif
#ifndef _IONBF
#define _IONBF            0x0004
#endif

Sd2Card card;
SdVolume volume;
SdFile root;

//String extensionProfiles = ".txt"; <--- C++ WAY
#define SLASH_WITH_EOF_STR "/"
#define extensionProfiles ".db"

/*! @brief Convierte a nombre de perfil a PATH (de manera normalizada, partiendo de raiz) usando los macros 'SLASH_WITH_EOF_STR' y 'extensionProfiles' 
    @param string nombre de perfil a convertir en directorio
    @return puntero al string PATH*/
#define profilePath(string) strcat(strcat(SLASH_WITH_EOF_STR, profileName),extensionProfiles)

/*! @brief Inicializador de la SD
    @note chequea todos los parametros. Y si no cumplen, dictara los problemas dados por el serial.
*/
void SDBegin(void);

namespace Profiles{
    /*! @brief Hace lectura del nombre de los perfiles dentro del almacenamiento  
        @returns Doble puntero estatico que apunta a vector de punteros. Cada puntero del vector apunta a los strings (que son los nombres de los perfiles)
     */
    char** showProfiles_(void);
    
    /*! @brief Crea un almacenamiento para un nuevo perfil
        @param name
               Nombre del perfil                            */
    void createProfile_(const char* name);
    /*! @brief Elimina el almacenamiento de un perfil
        @param name
               Nombre del perfil                            */
    void deleteProfile_(const char* name);

};

namespace SubProfiles{
    /*! @brief Crea un tipo de estructura estatica que es = Irdata + Nombre del Subperfil (Tipo Char[20])
     *  @param storedIRData 
     *         Recibe estructura normalizada de la informacion del infrarrojo
     *  @note Recordar eliminar la memoria dinamica reservada luego de su uso
     *  @returns Tipo de estructura con el agregado mencionado
    */
    Keep_t convertIRData(storedIRDataStruct* storedIRData);

    /*! @brief Crea un subperfil para un perfil
     *  @param subProfilename
     *         Nombre del subperfil
     *  @param storedIRData
     *          Estructura con la DATA del infrarrojo
     *  @param profileName
     *           Nombre del perfil en el que quiere guardar el subperfil
    */
    [[deprecated("En desuso porque no se usara en la version Pre-Alpha")]]
    void createSubProfile_(const char* subProfileName, storedIRDataStruct* storedIRData, const char* profileName);

    /*! @brief Hace lectura del nombre de los sub-perfiles dentro del almacenamiento del perfil dado
     *  @param name
     *         Nombre del perfil
     *  @returns Doble puntero estatico que apunta a vector de punteros. Cada puntero del vector apunta a los strings (que son los nombres de los subperfiles) 
    */
    char** showSubProfiles(const char* profileName);

    /*! @brief Busca dentro del almacenamiento del perfil dado, el subperfil solicitado
     *  @param profileName nombre del perfil en el que se encontrara el subperfil
     *  @param subProfileName nombre del subperfil que representara la DATA de la señal infrarroja
     *  @note usara la estructura recibida para almacenarla en la SD como subperfil de perfil anteriormente creado
     *  @returns Puntero Estatico de la estructura de datos del subperfil solicitado
    */
    Keep_t* ReturnSubProfile(const char* profileName, const char* subProfileName);


    /*! @brief Guarda en el almacenamiento el subPerfil recibido
        @param  storeIR estructura Normalizada con los datos a almacenar
        @param profileName nombre del perfil en el que se almacenará el subperfil
    */
    void storeSubProfile(Keep_t storeIR, const char* profileName);

    /*! @brief  Elimina en el almacenamiento el subPerfil recibido
     *   @param profileName nombre del perfil en el que se encontrara el subperfil
     *   @param subProfileName nombre del subperfil que representara la DATA de la señal infrarroja
     *   @note Borra la estructura que almacena el nombre del subperfil
    */
    void deleteSubProfile(const char* profileName, const char* subProfileName);

};

#endif //Profiles_hpp