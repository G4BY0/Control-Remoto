//Copyright Grupo 7, Inc. All Rights Reserved.
/*************************************************************
 * \file Header de PinOut del Atmel Atmega328p
 * #####PROBLEMA####, AL ESTAR DESARROLLANDO SOLAMENTE CON EL NODEMCU ESP32S, NO SE SABE SI EL RESTO DE LAS PLACAS LO SOPORTAN!

! @brief Agrupacion de los pines a utilizar dependiendo la placa
*************************************************************/


#ifndef PIN_H
#define PIN_H

#include <stdint.h>
#include "Board.h"

//Placa actualmente en uso y usada para el debugging del proyecto
#if defined ( ESP32 ) 

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
    constexpr uint8_t UP     = 13;
    constexpr uint8_t DOWN   = 12;
    constexpr uint8_t LEFT   = 14;
    constexpr uint8_t RIGHT  = 27;
    constexpr uint8_t BACK   = 26;
    constexpr uint8_t ENTER  = 25;
  };

  namespace SD_t{
    constexpr uint8_t chipSelect = 5;
  }

};

#elif defined( ESP8266 ) 
#warning "Este proyecto como se encuentra en desarrollo, esta siendo testeado en el Nodemcu ESP32. Por lo tanto, no se sabe del todo si aun esta hecho para el tipo de placas"
namespace PIN{
  
  namespace Display{
    constexpr uint8_t SDA    = 2;
    constexpr uint8_t SDL    = 1;
  };

  namespace InfraredReceiver{
    constexpr uint8_t DATA   = 0;     
  };

  namespace InfraredTransmitter{
    constexpr uint8_t DATA   = A0;  
  };

  namespace Buttons{
    constexpr uint8_t UP     = 6;
    constexpr uint8_t DOWN   = 5;
    constexpr uint8_t LEFT   = 4;
    constexpr uint8_t RIGHT  = 3;
    constexpr uint8_t BACK   = 2;
    constexpr uint8_t ENTER  = 1;
  };

  namespace SD_t{
    constexpr uint8_t chipSelect = 15;  
  }

};

#elif defined( ARDUINO_AVR_MEGA )
#warning "Este proyecto como se encuentra en desarrollo, esta siendo testeado en el Nodemcu ESP32. Por lo tanto, no se sabe del todo si aun esta hecho para el tipo de placas AVR"
namespace PIN{
  
  namespace Display{
    constexpr uint8_t SDA    = 20;
    constexpr uint8_t SDL    = 21;
  };

  namespace InfraredReceiver{
    constexpr uint8_t DATA   = 2;
  };

  namespace InfraredTransmitter{
    constexpr uint8_t DATA   = 3;
  };

  namespace Buttons{
    constexpr uint8_t UP     = A0;
    constexpr uint8_t DOWN   = A3;
    constexpr uint8_t LEFT   = A1;
    constexpr uint8_t RIGHT  = A2;
    constexpr uint8_t BACK   = A4;
    constexpr uint8_t ENTER  = A5;
  };

  namespace SD_t{
    constexpr uint8_t chipSelect = 53;
  }

};

#elif defined(ARDUINO_AVR_LEONARDO)
#warning "Este proyecto como se encuentra en desarrollo, esta siendo testeado en el Nodemcu ESP32. Por lo tanto, no se sabe del todo si aun esta hecho para el tipo de placas AVR"
namespace PIN{
  
  namespace Display{
    constexpr uint8_t SDA    = 2;
    constexpr uint8_t SDL    = 3;
  };

  namespace InfraredReceiver{
    constexpr uint8_t DATA   = 4;     
  };

  namespace InfraredTransmitter{
    constexpr uint8_t DATA   = 5;  
  };

  namespace Buttons{
    constexpr uint8_t UP     = A0;
    constexpr uint8_t DOWN   = A1;
    constexpr uint8_t LEFT   = A2;
    constexpr uint8_t RIGHT  = A3;
    constexpr uint8_t BACK   = A5;
    constexpr uint8_t ENTER  = A4;
  };

  namespace SD_t{
    constexpr uint8_t chipSelect = 10;
  }

};

#endif

#endif // PIN_H