#warning "Project is in developing, it's not already yet!"
#warning "Joaco estuvo aqui..."

#include <Arduino.h>
#include <Wire.h>
#include "Board.h"


#include "Modes.hpp"

using namespace MODE; // Implemento los modos


void setup(){
    
    Serial.begin(115200);

    while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
    }

    #if defined(__AVR_ATmega32U4__) || defined(SERIAL_PORT_USBVIRTUAL) || defined(SERIAL_USB) /*stm32duino*/|| defined(USBCON) /*STM32_stm32*/|| defined(SERIALUSB_PID) || defined(ARDUINO_attiny3217)
    delay(4000); // To be able to connect Serial monitor after reset or power up and before first print out. Do not wait for an attached Serial Monitor!
    #endif
    
    //Aviso del compilador utilizado (usando los identificadores de cada uno)
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

    //Inicializacion del sistema del display
    displayBegin();

    Serial.println("Hasta aca llegue!1");
    delay(3000);

    //Inicializacion del sistema de botones
    buttonsBegin();
    Serial.println("Hasta aca llegue!2");
    delay(3000);

    //Inicializacion del sistema de almacenamiento
    
    Serial.println("Hasta aca llegue!3");
    delay(3000);

    //Inicializacion del sistema del infrarrojo
    infraredBegin();

    Serial.println("Hasta aca llegue!4");
    delay(3000);
}

//Parte todo del Hub y luego se ramifica en los demas Menus
void loop(){  hub__(); }