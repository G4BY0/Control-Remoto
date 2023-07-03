//Copyright Grupo 11, Inc. All Rights Reserved.
/*************************************************************
 * \file Establecimiento de pines de la placa a Utilizar
 * 
 * #####PROBLEMA##### 
 * AL ESTAR DESARROLLANDO SOLAMENTE CON EL NODEMCU ESP32S, NO SE SABE SI EL RESTO DE LAS PLACAS LO SOPORTAN!
 * 
 * @brief Agrupacion de los pines a utilizar dependiendo la placa
*************************************************************/


#ifndef PIN_H
#define PIN_H

#include <stdint.h>

#define ESP_BOARDS defined ( ESP32 ) || defined( ESP8266 )

#if defined ( ESP32 ) 
//Placa actualmente en uso y usada para el debugging del proyecto
namespace PIN{
  
  namespace Display{
    const uint8_t SDA    = 21U;
    const uint8_t SDL    = 22U;
  };

  namespace InfraredReceiver{
    const uint8_t DATA   = 15U;     
  };

  namespace InfraredTransmitter{
    const uint8_t DATA   = 4U;  
  };

  namespace Buttons{
    constexpr uint8_t UP     = 13U;
    constexpr uint8_t DOWN   = 12U;
    constexpr uint8_t LEFT   = 14U;
    constexpr uint8_t RIGHT  = 27U;
    constexpr uint8_t BACK   = 26U;
    constexpr uint8_t ENTER  = 25U;
  };

  namespace SD_t{
    constexpr uint8_t chipSelect = 5U;
  }

};

#elif defined( ESP8266 ) 
#warning "Este proyecto como se encuentra en desarrollo, esta siendo testeado en el Nodemcu ESP32. Por lo tanto, no se sabe del todo si aun esta hecho para el tipo de placas"
namespace PIN{
  
  namespace Display{
    constexpr uint8_t SDA    = 2U;
    constexpr uint8_t SDL    = 1U;
  };

  namespace InfraredReceiver{
    constexpr uint8_t DATA   = 0U;     
  };

  namespace InfraredTransmitter{
    constexpr uint8_t DATA   = A0;  
  };

  namespace Buttons{
    constexpr uint8_t UP     = 6U;
    constexpr uint8_t DOWN   = 5U;
    constexpr uint8_t LEFT   = 4U;
    constexpr uint8_t RIGHT  = 3U;
    constexpr uint8_t BACK   = 2U;
    constexpr uint8_t ENTER  = 1U;
  };

  namespace SD_t{
    constexpr uint8_t chipSelect = 15U;  
  }

};

#elif defined( ARDUINO_AVR_MEGA )
#warning "Este proyecto como se encuentra en desarrollo, esta siendo testeado en el Nodemcu ESP32. Por lo tanto, no se sabe del todo si aun esta hecho para el tipo de placas AVR"
namespace PIN{
  
  namespace Display{
    constexpr uint8_t SDA    = 20U;
    constexpr uint8_t SDL    = 21U;
  };

  namespace InfraredReceiver{
    constexpr uint8_t DATA   = 2U;
  };

  namespace InfraredTransmitter{
    constexpr uint8_t DATA   = 3U;
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
    constexpr uint8_t chipSelect = 53U;
  }

};

#elif defined(ARDUINO_AVR_LEONARDO)
#warning "Este proyecto como se encuentra en desarrollo, esta siendo testeado en el Nodemcu ESP32. Por lo tanto, no se sabe del todo si aun esta hecho para el tipo de placas AVR"
namespace PIN{
  
  namespace Display{
    constexpr uint8_t SDA    = 2U;
    constexpr uint8_t SDL    = 3U;
  };

  namespace InfraredReceiver{
    constexpr uint8_t DATA   = 4U;     
  };

  namespace InfraredTransmitter{
    constexpr uint8_t DATA   = 5U;  
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
    constexpr uint8_t chipSelect = 10U;
  }

};

#endif

#endif // PIN_H