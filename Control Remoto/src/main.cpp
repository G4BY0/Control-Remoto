#warning "Project is in developing, it's not already yet!"
#warning "Joaco estuvo aqui"
#include "Arduino.h"
#include "Interface.h"
#include "Profiles.hpp"
#include "Infrared.hpp"
#include "Modes.hpp"

void setup(){
    
    Wire.begin();
    
    Serial.begin(9600);
    Serial.println("Tipo de compilador Utilizado: ");

    #if defined(__AVR_ATmega32U4__) || defined(SERIAL_PORT_USBVIRTUAL) || defined(SERIAL_USB) /*stm32duino*/|| defined(USBCON) /*STM32_stm32*/|| defined(SERIALUSB_PID) || defined(ARDUINO_attiny3217)
    delay(4000); // To be able to connect Serial monitor after reset or power up and before first print out. Do not wait for an attached Serial Monitor!
    #endif

    //Aviso del compilador utilizado (usando los identificadores de cada uno)
    #pragma region Compilador_Usado
    #if defined(__GNUC__)
        Serial.println("GNU :)");
        Serial.print("Version del compilador de GNU es: ");
        Serial.println(__GNUC__);
    #elif defined(__clang__)
        Serial.println("CLANG !!");
        Serial.print("Version Principal del compilador de CLANG es: "); 
        Serial.println(__clang_major__);
        Serial.print("Version Secundaria del compilador de CLANG es: ");
        Serial.println(__clang_minor__);
        Serial.print("Nivel de parche del compilador de CLANG: "); 
        Serial.println(__clang_patchlevel__);
    #else
        Serial.println("Generico :((");
    #endif
    #pragma endregion
    
    infraredBegin();
    displayBegin();
    buttonsBegin();
    SDBegin();
    
}


//Parte todo del Hub y luego se ramifica en los demas Menus
void loop(){  MODE::hub__(); }



