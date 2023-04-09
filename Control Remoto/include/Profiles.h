#ifndef PROFILES_H
#define PROFILES_H

#include "PIN.h"
#include <SD.h>
#include "Interface.h" // INCHEQUEABLE, DESPUES AVERIGUAR

Sd2Card card;
SdVolume volume;
SdFile root;

// String extensionProfiles = ".txt"; <--- C++ WAY
#define SLASH_WITH_EOF_STR "/"
const char* extensionProfiles = ".txt";


/*! \deprecated en desuso por problemas con manejo de constantes, se borro la definicion.
    @brief Concatena primer puntero tipo char constante con el segundo del mismo tipo
    @param firstPart Parte inicial
    @param finalPart Parte final
    @returns String concatenado  */
inline char* strcatconst(const char* firstPart, const char* finalPart);

void SDBegin(void){

    // CODIGO DE INICIALIZACION DE LIBRERIAS UTILES
    // TESTEANDO SI LA TARJETA SD ESTA TRABAJANDO!
    if (!card.init(SPI_HALF_SPEED, PIN::SD_t::chipSelect)) {
        Serial.println("initialization failed. Things to check:");
        Serial.println("* is a card inserted?");
        Serial.println("* is your wiring correct?");
        Serial.println("* did you change the chipSelect pin to match your shield or module?");
        while (1);
    } else {
        Serial.println("Wiring is correct and a card is present.");
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

namespace Profiles{
    /*! @brief Desde modo administrador, muestra en el Serial una lista del contenido del directorio raiz   */
    void showProfiles_(void);
    /*! @brief Crea un almacenamiento para un nuevo perfil
        @param name
               Nombre del perfil                            */
    void createProfile_(const char* name);
    /*! @brief Elimina el almacenamiento de un perfil
        @param name
               Nombre del perfil                            */
    void deleteProfile_(const char* name);
    /*! @brief Crea un subperfil para un perfil
        @param name
               Nombre del subperfil                            */
    void createSubProfile_(const char* name);

};


#endif