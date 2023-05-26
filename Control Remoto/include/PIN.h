//Copyright Grupo 7, Inc. All Rights Reserved.
/*************************************************************
 * \file Header de PinOut del Atmel Atmega328p
 * #####PROBLEMA####, AL ESTAR VIENDO QUE MICROCONTROLADOR DEBEMOS USAR, LA CONFIGURACION DE LOS PINES PUEDE VARIAR!!!
# DISPLAY

  - VCC         = MICROCONTROLLER_VCC
  - SDA         = ANALOG PIN 4
  - SDL         = ANALOG PIN 5
  - GND         = MICROCONTROLLER_GND

# INFRARED RECEIVER

  - DATA        = DIGITAL PIN 13
  - VCC         = MICROCONTROLLER_VCC
  - GND         = MICROCONTROLLER_GND

# INFRARED TRANSMITTER

    - DATA      = DIGITAL PIN 12
    - VCC         = MICROCONTROLLER_VCC
    - GND         = MICROCONTROLLER_GND

# BUTTONS 
 // PULL UP INSTALATIONS

  - UPBUTTON    = DIGITAL PIN 9
  - DOWNBUTTON  = DIGITAL PIN 8
  - LEFTBUTTON  = DIGITAL PIN 7
  - RIGHTBUTTON = DIGITAL PIN 6
  - BACKBUTTON  = DIGITAL PIN 5
  - ENTERBUTTON = DIGITAL PIN 4

! @brief Agrupacion de los pines a utilizar del Atmega328p
*************************************************************/


#ifndef PIN_H
#define PIN_H

#include <stdint.h>
#include "Board.h"

#ifdef ARDUINOMEGA2560_CONFIGURATION
namespace PIN{
  
  namespace Display{

    const uint8_t SDA    = SDA;
    const uint8_t SDL    = SDL;

  };

  namespace InfraredReceiver{

    constexpr const uint8_t DATA   = 2;     

  };

  namespace InfraredTransmitter{

    constexpr const uint8_t DATA   = 3;  

  };

  namespace Buttons{

    constexpr const uint8_t UP     = 9;
    constexpr const uint8_t DOWN   = 8;
    constexpr const uint8_t LEFT   = 7;
    constexpr const uint8_t RIGHT  = 6;
    constexpr const uint8_t BACK   = 5;
    constexpr const uint8_t ENTER  = 4;

  };

  namespace SD_t{

    // change this to match your SD shield or module;
    // Arduino Ethernet shield: pin 4
    // Adafruit SD shields and modules: pin 10
    // Sparkfun SD shield: pin 8
    // MKRZero SD: SDCARD_SS_PIN
    constexpr const uint8_t chipSelect = 53;
    

  }

};
#endif

#ifdef NODEMCUV2ESP8266_CONFIGURATION
namespace PIN{
  
  namespace Display{

    const uint8_t SDA    = 2;
    const uint8_t SDL    = 1;

  };

  namespace InfraredReceiver{

    constexpr const uint8_t DATA   = 0;     

  };

  namespace InfraredTransmitter{

    constexpr const uint8_t DATA   = A0;  

  };

  namespace Buttons{

    constexpr const uint8_t UP     = 6;
    constexpr const uint8_t DOWN   = 5;
    constexpr const uint8_t LEFT   = 4;
    constexpr const uint8_t RIGHT  = 3;
    constexpr const uint8_t BACK   = 2;
    constexpr const uint8_t ENTER  = 1;

  };

  namespace SD_t{

    // change this to match your SD shield or module;
    // Arduino Ethernet shield: pin 4
    // Adafruit SD shields and modules: pin 10
    // Sparkfun SD shield: pin 8
    // MKRZero SD: SDCARD_SS_PIN
    constexpr const uint8_t chipSelect = 15;
    

  }

};
#endif

#ifdef STM32F103C8_GENERICBOARD_CONFIGURATION

#endif

#endif // PIN_H