/*********************************************************************************************************************************************
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

********************************************************************************************************************************************/


#ifndef PIN_H
#define PIN_H

#include <Arduino.h>

namespace PIN{
  
  namespace Display{

    const uint8_t SDA    = A4;
    const uint8_t SDL    = A5;

  };

  namespace InfraredReceiver{

    const uint8_t DATA   = 13;     

  };

  namespace InfraredTransmitter{

    const uint8_t DATA   = 12;  

  };

  namespace Buttons{

    const uint8_t UP     = 9;
    const uint8_t DOWN   = 8;
    const uint8_t LEFT   = 7;
    const uint8_t RIGHT  = 6;
    const uint8_t BACK   = 5;
    const uint8_t ENTER  = 4;

  };

  namespace SD_t{

    // change this to match your SD shield or module;
    // Arduino Ethernet shield: pin 4
    // Adafruit SD shields and modules: pin 10
    // Sparkfun SD shield: pin 8
    // MKRZero SD: SDCARD_SS_PIN
    const uint8_t chipSelect = 4;


  }


};

#endif