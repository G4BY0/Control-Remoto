#warning "Project is in developing, it's not already yet!"

#include <Arduino.h>
#include <Wire.h>

#include "Modes.hpp"

using namespace MODE; // Implemento los modos


void setup(){
    
    Wire.begin();
    
    Serial.begin(9600);

    #if defined(__AVR_ATmega32U4__) || defined(SERIAL_PORT_USBVIRTUAL) || defined(SERIAL_USB) /*stm32duino*/|| defined(USBCON) /*STM32_stm32*/|| defined(SERIALUSB_PID) || defined(ARDUINO_attiny3217)
    delay(4000); // To be able to connect Serial monitor after reset or power up and before first print out. Do not wait for an attached Serial Monitor!
    #endif
    
    //Aviso del compilador utilizado (usando los identificadores de cada uno)
    #pragma region Compilador_Usado
    Serial.println(F("Tipo de compilador Utilizado: "));

    #if defined(__GNUC__)
        Serial.println(F("GNU :)"));
        Serial.print(F("Version del compilador de GNU es: "));
        Serial.println(__GNUC__);
    #elif defined(__clang__)
        Serial.println(F("CLANG !!"));
        Serial.print(F("Version Principal del compilador de CLANG es: ")); 
        Serial.println(__clang_major__);
        Serial.print(F("Version Secundaria del compilador de CLANG es: "));
        Serial.println(__clang_minor__);
        Serial.print(F("Nivel de parche del compilador de CLANG: ")); 
        Serial.println(__clang_patchlevel__);
    #else
        Serial.println(F("Generico :(("));
    #endif
    #pragma endregion
    
    //Inicializacion del sistema del display
    displayBegin();

    //Inicializacion del sistema de botones
    buttonsBegin();

    //Inicializacion del sistema de almacenamiento
    SDBegin();

    //Inicializacion del sistema del infrarrojo
    infraredBegin();

    
}

//Parte todo del Hub y luego se ramifica en los demas Menus
void loop(){  hub__(); }