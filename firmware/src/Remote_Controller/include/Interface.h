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
 
#include <cstdint>
#include <algorithm>
#include <pgmspace.h>
#include <Arduino.h>
#include <SD.h>
#include <vector>
#include <array>
#include <SPI.h>  //SPI
#include <Wire.h> //I2C
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#include "PIN.h"
#include "System.h"
#include "Tools/Cursor.h"
#include "Tools/Writter.h"
#include "Tools/Book.h"
#include "Infrared.h"
#include "Storage/Profiles.h"


//Objeto para el manejo de la pantalla a utilizar (tecnologia OLED con driver SH1106G)
extern Adafruit_SH1106G display;

//Macros para el desarrollo del sistema
#define INFINITE_LOOPING 0x1
#define SYSTEM_STRING_ERROR "SystemError"

//  DISPLAY OLED MACROS!
// Display OLED Adafruit_SH110X
/* Uncomment the initialize the I2C address , uncomment only one, If you get a totally blank screen try the other*/
#define I2C_ADDRESS 0x3c // initialize with the I2C addr 0x3C Typically eBay OLED's
// #define i2c_Address 0x3d //initialize with the I2C addr 0x3D Typically Adafruit OLED's

// Oled display size
#define SCREEN_WIDTH  (128U) // OLED display width, in pixels
#define SCREEN_HEIGHT (64U)  // OLED display height, in pixels

#define OLED_RESET -1    //   QT-PY / XIAO
#define BLINK_TIME 500   // Exclusivo para hacer parpadear un texto

//Pulsadores
/*! \brief Estado logico de la botonera
  \param PIN_BUTTON PIN de la placa conectada al boton
  \note Instalacion PullUP
  \returns Si esta presionado 'true', sino 'false' */
#define buttonState(PIN_BUTTON) !digitalRead(PIN_BUTTON) 

#define DEBOUNCE_TIME 300 //Tiempo de seguridad por el rebote mecanico de los pulsadores
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

  /*! \brief Menu de edicion de perfiles y subperfiles
      \note Generara una interacion entre el usuario otorgandole permisos de movimiento de perfiles y subperfiles*/
  void edit(void);

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

  
  /*! \brief    Interfaz del modo "Crear SubPerfil (Pre-Alpha)" almacenados" a la salida del display
  */
  void addSubProfile(std::string profileSelected = std::string(""));

  /*! \brief    Interfaz del modo "Eliminar SubPerfil (Pre-Alpha)" a la salida del display
  */
  void deleteSubProfile(std::string profileSelected = std::string(""));
  
  /*! \brief    Interfaz del modo "Esperando Infrarrojo" 
      \return   'true' Si cumplió correctamente.
                'false' Si el usuario canceló el modo.
      \note Entra en bucle hasta vericar que el codigo recibido es correcto.
  */
  bool waitingForIR(void);

  /*! \brief Muestra la bateria en la pantalla
      \note Se muestra en alguna de las esquinas de la pantalla*/
  void battery(void);

  /*! \brief Decodifica la estructura recibida y devuelve tiempo en string
      \param rtc Objeto de Registro del Tiempo
      \param buff Buffer en el que se llenara con el string de time dado
      \note Hace la copia del string en el buffer, "%s%2i:%2i:%2i" <---- Example: thu 05:08:34. No tiene en cuenta el size del buffer, usar con precaucion.*/
  void clock( const struct tm& time , char* buff);

  namespace EmergencyCalls {

    /*! \brief  Ventana Emergente al display que indica: "No hay perfiles almacenados" a la salida del display
        \note Tambien avisa por Serial
    */
    void nonProfiles(void);

    /*! \brief  Ventana Emergente al display que indica: "No hay sub-perfiles almacenados" a la salida del display
        \note Tambien avisa por Serial
    */
    void nonSubProfiles(void);

    /*! \brief  Ventana Emergente al display que indica: "No se pudo crear el Perfil" a la salida del display
        \note Tambien avisa por Serial
    */
    void noProfileCreated(void);

    /*! \brief  Ventana Emergente al display que indica: "No se pudo crear el Sub-Perfil" a la salida del display
        \note Tambien avisa por Serial
    */
    void noSubProfileCreated(void);

    /*! \brief  Ventana Emergente al display que indica: "No se pudo borrar el Perfil" a la salida del display
        \note Tambien avisa por Serial
    */
    void noProfileDeleted(void);

    /*! \brief  Ventana Emergente al display que indica: "No se pudo borrar el Sub-Perfil" a la salida del display
        \note Tambien avisa por Serial
    */
    void noSubProfileDeleted(void);

  };

};

#endif //Interface_h