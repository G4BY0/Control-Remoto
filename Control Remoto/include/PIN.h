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

    const uint8_t SDA    = 20;
    const uint8_t SDL    = 21;

  };

  namespace InfraredReceiver{

    constexpr const uint8_t DATA   = 2;

  };

  namespace InfraredTransmitter{

    constexpr const uint8_t DATA   = 3;

  };

  namespace Buttons{

    constexpr const uint8_t UP     = A0;
    constexpr const uint8_t DOWN   = A3;
    constexpr const uint8_t LEFT   = A1;
    constexpr const uint8_t RIGHT  = A2;
    constexpr const uint8_t BACK   = A4;
    constexpr const uint8_t ENTER  = A5;

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

#elif defined(  NODEMCUV2ESP8266_CONFIGURATION ) //Deprecated por falta de comunicacion I2C
#warning "Solamente Desarrollado Para Nodemcu Esp32s (ESP32-WROOM-32)"
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

#elif defined( NODEMCUESP32S_CONFIGURATION )

namespace PIN{
  
  namespace Display{

    const uint8_t SDA    = 21;
    const uint8_t SDL    = 22;

  };

  namespace InfraredReceiver{

    const uint8_t DATA   = 15;     

  };

  namespace InfraredTransmitter{

    const uint8_t DATA   = 4;  

  };

  namespace Buttons{

    const uint8_t UP     = 13;
    const uint8_t DOWN   = 12;
    const uint8_t LEFT   = 14;
    const uint8_t RIGHT  = 27;
    const uint8_t BACK   = 26;
    const uint8_t ENTER  = 25;

  };

  namespace SD_t{

    // change this to match your SD shield or module;
    // Arduino Ethernet shield: pin 4
    // Adafruit SD shields and modules: pin 10
    // Sparkfun SD shield: pin 8
    // MKRZero SD: SDCARD_SS_PIN
    const uint8_t chipSelect = 5;
    

  }

};

#elif defined(ARDUINOLEONARDO_CONFIGURATION)

namespace PIN{
  
  namespace Display{

    const uint8_t SDA    = 2;
    const uint8_t SDL    = 3;

  };

  namespace InfraredReceiver{

    const uint8_t DATA   = 4;     

  };

  namespace InfraredTransmitter{

    const uint8_t DATA   = 5;  

  };

  namespace Buttons{

    const uint8_t UP     = A0;
    const uint8_t DOWN   = A1;
    const uint8_t LEFT   = A2;
    const uint8_t RIGHT  = A3;
    const uint8_t BACK   = A5;
    const uint8_t ENTER  = A4;

  };

  namespace SD_t{

    // change this to match your SD shield or module;
    // Arduino Ethernet shield: pin 4
    // Adafruit SD shields and modules: pin 10
    // Sparkfun SD shield: pin 8
    // MKRZero SD: SDCARD_SS_PIN
    const uint8_t chipSelect = 10;
    

  }

};

#elif defined( STM32F103C8_GENERICBOARD_CONFIGURATION )
#warning "Solamente Desarrollado Para Nodemcu Esp32s (ESP32-WROOM-32)"
#endif

#endif // PIN_H