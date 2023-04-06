/********************************************************************************************************************************************************
  -INTERFAZ GRAFICA DEL DISPLAY
  -INICIALIZACIONES
  -MENUS
  -CURSOR DEL USUARIO
  
  *********************************************************************************************************************
  CORRECCIONES DE ERORRES DE LOS ENUM, FALTABA PONER EL TIPO DE ENUM QUE ERA. ES DECIR, Enum pedro{}; Estaba solo Enum
  *********************************************************************************************************************

*********************************************************************************************************************************************************/


#ifndef INTERFACE_H 
#define INTERFACE_H
 

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <SD.h>
#include "PIN.h"
#include "Profiles.h"

#define INFINITE_LOOPING 0x1
#define SYSTEM_STRING_ERROR "SystemError"

#define MODE_HUB "Hub"
#define MODE_PROFILES "Profiles"
#define MODE_ADDPROFILE "addProfile"
#define MODE_DELETEPROFILE "deleteProfile"

const enum MODE_t{ HUB, PROFILES, ADDPROFILE, DELETEPROFILE };

const enum BUTTON_PRESSED{ ENTER, BACK };


/* Uncomment the initialize the I2C address , uncomment only one, If you get a totally blank screen try the other*/
#define I2C_ADDRESS 0x3c // initialize with the I2C addr 0x3C Typically eBay OLED's
// #define i2c_Address 0x3d //initialize with the I2C addr 0x3D Typically Adafruit OLED's

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1    //   QT-PY / XIAO

#define BUTTON_BACK_PRESSED 0x0

#define TRUE_PULLUP 0x0
#define TRUE_PULLDOWN 0x1
#define FALSE_PULLUP 0x1
#define FALSE_PULLDOWN 0x0

#define SPACE_FOR_PUSSYS '_'
#define LINE_STRING_X 20

const uint8_t LINE_STRING_Y[] = {10,20,50};




/*! @brief Inicializacion de botonera
    @note Arduino por defecto establece como entrada los pines digitales*/
void buttonsBegin(void);

/*! @brief Estado logico del pin de la placa de desarrollo
    @param pin 
            Pin de la placa de desarrollo
    @returns Estado logico del pin de la placa de desarrollo */
inline bool buttonState(const uint8_t PIN_BUTTON);

Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

/*! @brief Cursor que se desplaza a traves del display
    @note Posee de dos constructores
    @note 1- Para desplazarse entre los menus del display
    @note 2- Para la creacion de strings que recibe del usuario*/
class Cursor {

private:
  const uint8_t LINE_CURSOR_X = 10;
  const char CURSOR_CHARACTER = '>';
  const char CURSOR_CHARACTER_DELETE = 'X';
  
  uint8_t AMOUNT_OF_OPTIONS;

public:
  /*! @brief Constructor para uso de desplazamiento de opciones
      @param  AMOUNT_OF_OPTIONS
      Cantidad de opciones que puede desplazarse el cursor  */
  Cursor(const uint8_t AMOUNT_OF_OPTIONS__);
  /*! @brief Constructor para uso de almacenamiento de strings del usuario  */
  Cursor(void);
  /*! @brief Cursor funcional con sus respectivos pulsadores
      @return El numero de la opcion seleccionada (en caso de presionar el boton BACK, retornara 0) */
  const int8_t options(void);

  const char* writer_ptr(void);

};

/*! @brief Inicializacion de display  */
void displayBegin(void);

namespace Interface {

  const enum MODE_t hub(void);

  const enum MODE_t profiles(void);

  const enum MODE_t addProfile(void);

  const enum MODE_t deleteProfile(void);

};


#endif