//Copyright Grupo 11, Inc. All Rights Reserved.
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

#include <cstdint>
#include <string>
#include <vector>
#include <memory>

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

#include "PIN.h"
#include "Infrared.h"

// Path del archivo que se usara para poder sobreescribir perfiles
#define TRANSFER_FILE_DIRANDNAME "/Transfer.db"

// Los perfiles se almacenan en archivos individuales ".db"
#define SLASH_WITH_EOF_STR "/"
#define extensionProfiles ".db"

enum class Protocols{
    IR,                 //INFRARED
    WIFI,               //WIFI
    BLUETOOTH           //BLUETOOTH
};

/*! \brief Convierte a nombre de perfil a PATH (de manera normalizada, partiendo de raiz) usando los macros 'SLASH_WITH_EOF_STR' y 'extensionProfiles'
    \param string nombre de perfil a convertir en directorio
    \return puntero al string PATH del perfil recibido*/
const char* profilePath(const char* profileName);

/*! \brief Inicializador de la SD
    \note chequea todos los parametros. Y si no cumplen, dictara los problemas dados por el serial.
*/
void SDBegin(void);

namespace Profiles{
    /*! \brief Hace lectura del nombre de los perfiles dentro del almacenamiento  
        \returns Doble puntero estatico que apunta a vector de punteros. Cada puntero del vector apunta a los strings (que son los nombres de los perfiles)
     */
    std::vector<std::string> showProfiles_(void);
    
    /*! \brief Crea un almacenamiento para un nuevo perfil
        \param name Nombre del perfil
        \returns Booleano si se cumplio satisfactoriamente o si falló       
        \note Si cumplió 'true' si falló 'false'       */
    bool createProfile_(const char* name);
    /*! \brief Elimina el almacenamiento de un perfil
        \param name Nombre del perfil
        \returns Booleano si se cumplio satisfactoriamente o si falló       
        \note Si cumplió 'true' si falló 'false'       */
    bool deleteProfile_(const char* name);

};

namespace SubProfiles{

    using ::storedIRDataStruct;
    using ::Protocols;

    /*! \deprecated En desuso porque no se usara en la version Pre-Alpha
     *  \brief Crea un subperfil para un perfil
     *  \param subProfilename Nombre del subperfil
     *  \param storedIRData Portador de la informacion de la señal
     *  \param profileName Nombre del perfil en el que quiere guardar el subperfil
    */
    void createSubProfile_(const char* subProfileName, Protocols protocol , const char* profileName);
 
    /*! \brief Hace lectura del nombre de los sub-perfiles dentro del almacenamiento del perfil dado
     *  \param name Nombre del perfil
     *  \returns Vector dinamico de std::string (Cada string es el nombre de un subperfil)
     *  \note Si hay un problema o no hay ningun subperfil, el puntero apuntara 'nullptr'
    */
    std::vector<std::string> showSubProfiles(const char* profileName);

    /*! \brief Busca dentro del almacenamiento del perfil dado, el subperfil solicitado
     *  \param profileName Nombre del perfil en el que se encontrara el subperfil
     *  \param subProfileName Nombre del subperfil que representara la DATA de la señal infrarroja
     *  \returns Puntero inteligente de la estructura de datos del subperfil solicitado
     *  \note Si hay un problema o no lo encuentra, el puntero apuntara 'nullptr'
    */
    std::shared_ptr<storedIRDataStruct> ReturnSubProfile(const char* profileName, const char* subProfileName);

    /*! \brief  Elimina en el almacenamiento el subPerfil recibido
     *  \param profileName Nombre del perfil en el que se encontrara el subperfil
     *  \param subProfileName Nombre del subperfil que representara la DATA de la señal infrarroja
     *  \note Borra la estructura que almacena la informacion del subperfil.
    */
    void deleteSubProfile(const char* profileName, const char* subProfileName);

};

#endif //Profiles_hpp