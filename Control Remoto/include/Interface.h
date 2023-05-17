//Copyright Grupo 7, Inc. All Rights Reserved.
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
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <IRremote.h>
#include <SD.h>

#include "PIN.h"
#include "Profiles.hpp"
#include "Cursor.h"

#define INFINITE_LOOPING 0x1
#define SYSTEM_STRING_ERROR "SystemError"

#define MODE_HUB 0x0              /* LOBBY*/
#define MODE_PROFILES 0x1         /*PERFILES*/
#define MODE_ADDPROFILE 0x2       /*CREAR PERFILES*/
#define MODE_DELETEPROFILE 0x3    /*ELIMINAR PERFILES*/
#if 0
  /*Posibles modos futuros*/
  #define MODE_HELP 0x4             /* Muestra como funciona el sistema y explica como va la cosa*/
#endif
 
#define BUTTON_PRESSED_ENTER 0x0
#define BUTTON_PRESSED_BACK 0x1
#ifndef buttonState_function_Macro
  #define buttonState_function_Macro
  /*! @brief Estado logico del pin de la placa de desarrollo
    @param PIN_BUTTON 
           Pin de la placa de desarrollo
    @returns Estado logico del pin de la placa de desarrollo */
  #define buttonState(PIN_BUTTON) digitalRead(PIN_BUTTON) 
#endif


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

#define MAX_LINE_OPTIONS_OUTPUT 3
#define SPACE_FOR_PUSSYS '_'
#define LINE_STRING_X 20

//region Blink_PulseAnyBotton
#define BLINK_TIME 500


const uint8_t LINE_STRING_Y[MAX_LINE_OPTIONS_OUTPUT] = {10,20,50};

/*! @brief Inicializacion de botonera
    @note Arduino por defecto establece como entrada los pines digitales*/
void buttonsBegin(void);

//Objeto para el manejo del Display OLED
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

/*! @brief Inicializacion de display  */
void displayBegin(void);

namespace Interface {

  /*! @brief   Interfaz del lobby a la salida del display
      @returns Numero normalizado que indica el modo seleccionado*/
  uint8_t hub(void);

  /*! @brief    Interfaz del modo "profiles" a la salida del display 
  */
  void profiles(void);

  /*! @brief    Interfaz del modo "addProfile" a la salida del display
  */
  void addProfile(void);

  /*! @brief    Interfaz del modo "deleteProfile" a la salida del display
  */
  void deleteProfile(void);

  /*! @brief    Interfaz del modo "sub-profiles" a la salida del display
      @param    profileName_ Nombre del perfil (tipo String)
  */
  void subProfiles(const char *profileName_);

  /*! @brief    Interfaz del modo "No hay perfiles almacenados" a la salida del display
  */
  void nonProfiles(void);

  /*! @brief    Interfaz del modo "No hay sub-perfiles almacenados" a la salida del display
  */
  void nonSubProfiles(void);

  //#pragma region Pre-Alpha // Version Primera de Salida
  /*! @brief    Interfaz del modo "Crear SubPerfil (Pre-Alpha)" almacenados" a la salida del display
  */
  void createSubProfile(void);

  /*! @brief    Interfaz del modo "Eliminar SubPerfil (Pre-Alpha)" a la salida del display
  */
  void deleteSubProfile(void);
  //#pragma endregion // Fin de funciones agregadas para Pre-Alpha

};

#endif //Interface_h