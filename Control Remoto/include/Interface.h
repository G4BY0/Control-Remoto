//Copyright Grupo 11, Inc. All Rights Reserved.
/***********************************************
 * * * * * * * * * * * * * * * * * * * * * * * *
 * \file
 * -INTERFAZ GRAFICA DEL DISPLAY
 * -INICIALIZACIONES
 * -MENUS
 * -CURSOR DEL USUARIO
 * 
 * * * * * * * * * * * * * * * * * * * * * * * *
***********************************************/
#ifndef INTERFACE_H 
#define INTERFACE_H
 
#include <stdint.h>
#include <Arduino.h>
#include <SPI.h>  //SPI
#include <Wire.h> //I2C
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <SD.h>
#include <vector>

#include "PIN.h"
#include "Cursor.h"
#include "Infrared.h"
#include "Profiles.hpp"

//Macros para el desarrollo del sistema
#define INFINITE_LOOPING 0x1
#define SYSTEM_STRING_ERROR "SystemError"

//Display OLED Adafruit_SH110X
/* Uncomment the initialize the I2C address , uncomment only one, If you get a totally blank screen try the other*/
#define I2C_ADDRESS 0x3c // initialize with the I2C addr 0x3C Typically eBay OLED's
// #define i2c_Address 0x3d //initialize with the I2C addr 0x3D Typically Adafruit OLED's
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1    //   QT-PY / XIAO
#define BLINK_TIME 500 //Exclusivo para hacer parpadear un texto

//Pulsadores


#define buttonState_function_Macro
/*! \brief Estado logico de la botonera
  \param PIN_BUTTON PIN de la placa conectada al boton
  \note Instalacion PullUP
  \returns Si esta presionado 'true', sino 'false' */
#define buttonState(PIN_BUTTON) !digitalRead(PIN_BUTTON) 

#define DEBOUNCE_TIME 500 //Tiempo de seguridad por el rebote mecanico de los pulsadores
#define TRUE_PULLUP 0x0
#define TRUE_PULLDOWN 0x1
#define FALSE_PULLUP 0x1
#define FALSE_PULLDOWN 0x0

/*! @brief Inicializacion de botonera
    @note Arduino por defecto establece como entrada los pines digitales*/
void buttonsBegin(void);

/*! \brief Inicializacion de display  */
void displayBegin(void);

//! \brief Manejo de la Interfaz de Usuario en cada modo
namespace Interface {

  /*! \brief   Interfaz del lobby a la salida del display
      \returns Numero normalizado que indica el modo seleccionado*/
  uint8_t hub(void);

  /*! \brief    Interfaz del modo "profiles" a la salida del display 
  */
  void profiles(void);

  /*! \brief    Interfaz del modo "addProfile" a la salida del display
  */
  void addProfile(void);

  /*! \brief    Interfaz del modo "deleteProfile" a la salida del display
  */
  void deleteProfile(void);

  /*! \brief    Interfaz del modo "sub-profiles" a la salida del display
      \param    profileName_ Nombre del perfil (tipo String)
  */
  void subProfiles(const char *profileName_);

  /*! \brief    Interfaz del modo "No hay perfiles almacenados" a la salida del display
  */
  void nonProfiles(void);

  /*! \brief    Interfaz del modo "No hay sub-perfiles almacenados" a la salida del display
  */
  void nonSubProfiles(void);

  
  /*! \brief    Interfaz del modo "Crear SubPerfil (Pre-Alpha)" almacenados" a la salida del display
  */
  void createSubProfile(void);

  /*! \brief    Interfaz del modo "Eliminar SubPerfil (Pre-Alpha)" a la salida del display
  */
  void deleteSubProfile(void);
  

  /*! \brief    Interfaz del modo "Esperando Infrarrojo" 
      \return   '1' Si el usuario canceló el modo.
                '0' Si cumplió correctamente.
      \note Entra en bucle hasta vericar que el codigo recibido es correcto.
  */
  bool waitingForIR(void);

  /*! \brief    Muestra las instrucciones de como se usa el dispositivo 
      \note   El usuario debe scanear un QR que lo regirira a una web */
  void help(void);


};

#endif //Interface_h