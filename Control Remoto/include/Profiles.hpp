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

//Polimorfismo de la estructura de la DATA del infrarrojo que implementa el nombre dado por el usuario
struct Keep : public storedIRDataStruct {

    char* nameSubProfile;

} keep;

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
const char* extensionProfiles = ".txt";

/*! \deprecated en desuso por problemas con manejo de constantes, se borro la definicion.
    @brief Concatena primer puntero tipo char constante con el segundo del mismo tipo
    @param firstPart Parte inicial
    @param finalPart Parte final
    @returns String concatenado  */
inline char* strcatconst(const char* firstPart, const char* finalPart);

/*! @brief Inicializador de la SD
    @note chequea todos los parametros. Y si no cumplen, dictara los problemas dados por el serial.
*/
void SDBegin(void);

namespace Profiles{
    /*! @brief Desde modo administrador, muestra en el Serial una lista del contenido del directorio raiz  
        @returns Vector de strings, la primera posicion del vector almacena
                 la cantidad de perfiles que tiene el directorio de cache
     */
    String* showProfiles_(void);
    
    /*! @brief Crea un almacenamiento para un nuevo perfil
        @param name
               Nombre del perfil                            */
    void createProfile_(const char* name);
    /*! @brief Elimina el almacenamiento de un perfil
        @param name
               Nombre del perfil                            */
    void deleteProfile_(const char* name);
    /*! @brief Crea un subperfil para un perfil
        @param subProfilename
               Nombre del subperfil
        @param storedIRData
                Estructura con la DATA del infrarrojo
        @param profileName
                Nombre del perfil en el que quiere guardar el subperfil*/
    void createSubProfile_(const char* name, storedIRDataStruct* storedIRData);

};


#endif //Profiles_hpp