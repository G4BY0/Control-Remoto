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
#include <pgmspace.h>

#define ESP_BOARDS defined ( ESP32 ) || defined( ESP8266 )

#if defined ( ESP32 ) 
//Placa actualmente en uso y usada para el debugging del proyecto
namespace PIN{
  
  namespace Energy{

    PROGMEM constexpr uint8_t BATTERY = 35U; 
    PROGMEM constexpr uint8_t BatteryCHARGE = 34U;
    PROGMEM constexpr uint8_t BatterySTANDBY = 15U;

  };

  namespace Display{
    PROGMEM constexpr uint8_t SDA    = 21U;
    PROGMEM constexpr uint8_t SDL    = 22U;
  };

  namespace InfraredReceiver{
    PROGMEM constexpr uint8_t DATA   = 13U;     
  };

  namespace InfraredTransmitter{
    PROGMEM constexpr uint8_t DATA   = 4U;  
  };

  namespace Buttons{
    PROGMEM constexpr uint8_t UP     = 26U;
    PROGMEM constexpr uint8_t DOWN   = 14U;
    PROGMEM constexpr uint8_t LEFT   = 32U;
    PROGMEM constexpr uint8_t RIGHT  = 27U;
    PROGMEM constexpr uint8_t BACK   = 33U;
    PROGMEM constexpr uint8_t ENTER  = 25U;
  };

  namespace SD_t{
    PROGMEM constexpr uint8_t chipSelect = 5U;
  }

};

#elif defined( ESP8266 ) 
#warning "Este proyecto como se encuentra en desarrollo, esta siendo testeado en el Nodemcu ESP32. Por lo tanto, no se sabe del todo si aun esta hecho para el tipo de placas"
namespace PIN{
  
  namespace Display{
    PROGMEM constexpr uint8_t SDA    = 2U;
    PROGMEM constexpr uint8_t SDL    = 1U;
  };

  namespace InfraredReceiver{
    PROGMEM constexpr uint8_t DATA   = 0U;     
  };

  namespace InfraredTransmitter{
    PROGMEM constexpr uint8_t DATA   = A0;  
  };

  namespace Buttons{
    PROGMEM constexpr uint8_t UP     = 6U;
    PROGMEM constexpr uint8_t DOWN   = 5U;
    PROGMEM constexpr uint8_t LEFT   = 4U;
    PROGMEM constexpr uint8_t RIGHT  = 3U;
    PROGMEM constexpr uint8_t BACK   = 2U;
    PROGMEM constexpr uint8_t ENTER  = 1U;
  };

  namespace SD_t{
    PROGMEM constexpr uint8_t chipSelect = 15U;  
  }

};

#endif

#endif // PIN_H