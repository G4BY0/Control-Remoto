//Copyright Grupo 11, Inc. All Rights Reserved.
/***********************************************
 * * * * * * * * * * * * * * * * * * * * * * * *
 * \file
 * Main.cpp (Main File)
 * 
 * * * * * * * * * * * * * * * * * * * * * * * *
***********************************************/
#warning "Project is in developing, it's not already yet!"

#include <Arduino.h>
#include <Wire.h> // I2C
#include "Modes.h"

SPIClass spi;

using namespace MODE; // Implemento los modos

void setup(){

    Serial.begin(115200);

    //Aviso del compilador utilizado (usando los identificadores de cada uno)
    auto UsedCompiler = [&]() -> void {
    Serial.println(F("Compilado el: " __DATE__ "\nTipo de compilador Utilizado: "));
    #if defined(__GNUC__)
        Serial.println(F("GNU :)"));
        Serial.print(F("Version del compilador de GNU es: ")); 
        Serial.println(F(__GNUC__));
    #elif defined(__clang__)
        Serial.println(F("CLANG !!"));
        Serial.println(F("Version Principal del compilador de CLANG es: "  __clang_major__ ));
        Serial.println(F("Version Secundaria del compilador de CLANG es: " __clang_minor__ ));
        Serial.println(F("Nivel de parche del compilador de CLANG: "       __clang_patchlevel__ ));
    #else
        Serial.println(F("Generico"));
    #endif
    };

    // Voy a usar los puertos de VSPI para la comunicacion SPI (Almacenamiento)
    spi=SPIClass(VSPI); 

    while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
    }

    //Problemas con el monitor Serial para las placas mencionadas
    #if defined(__AVR_ATmega32U4__) || defined(SERIAL_PORT_USBVIRTUAL) || defined(SERIAL_USB) /*stm32duino*/|| defined(USBCON) /*STM32_stm32*/|| defined(SERIALUSB_PID) || defined(ARDUINO_attiny3217)
    delay(4000); // To be able to connect Serial monitor after reset or power up and before first print out. Do not wait for an attached Serial Monitor!
    #endif

    //Descomentar en Caso de querer saber cual fue el compilador utilizado
    //UsedCompiler();

    //Inicializacion del sistema del display
    displayBegin();     Serial.println(F("Display Inicializado"));
    
    //Inicializacion del sistema de botones
    buttonsBegin();     Serial.println(F("Botonera Inicializada"));
    
    //Inicializacion del sistema de almacenamiento
    SDBegin();          Serial.println(F("Almacenamiento Inicializado"));
    
    //Inicializacion del sistema del infrarrojo
    infraredBegin();    Serial.println(F("Infrared Inicializado"));

    //Espero a que todos los procesos terminen para inicializar
    yield();
}

//Parte todo del Hub y luego se ramifica en los demas Menus
void loop(){ julio(); }