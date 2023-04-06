#ifndef PROFILES_H
#define PROFILES_H

#include "PIN.h"
#include <SD.h>
#include "Interface.h" // INCHEQUEABLE, DESPUES AVERIGUAR

Sd2Card card;
SdVolume volume;
SdFile root;




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
    void createProfile_(char* name);
    /*! @brief Elimina el almacenamiento de un perfil
        @param name
               Nombre del perfil                            */
    void deleteProfile_(char* name);
    /*! @brief Crea un subperfil para un perfil
        @param name
               Nombre del perfil                            */
    void Profiles::createSubProfile_(char* name);

};


#endif