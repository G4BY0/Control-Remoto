#ifndef PROFILES_H
#define PROFILES_H

#include "SD.h"

Sd2Card card;
SdVolume volume;
SdFile root;

// change this to match your SD shield or module;
// Arduino Ethernet shield: pin 4
// Adafruit SD shields and modules: pin 10
// Sparkfun SD shield: pin 8
// MKRZero SD: SDCARD_SS_PIN
const int chipSelect = 4;


void SDBegin(void){

    // CODIGO DE INICIALIZACION DE LIBRERIAS UTILES
    // TESTEANDO SI LA TARJETA SD ESTA TRABAJANDO!
    if (!card.init(SPI_HALF_SPEED, chipSelect)) {
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

};


#endif