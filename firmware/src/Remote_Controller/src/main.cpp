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
#include <Wire.h>               // I2C
#ifdef ESP32
#include <Wifi.h>
#elif ESP8266
#include <ESP8266WiFi.h>
#endif
#include <NTPClient.h>
#include <WiFiUdp.h>
#define WIFI_ON

#include <SPI.h> 
#include "System.h"
#include "Storage/Config.h"
#include "Modes/Modes.h"

#define DEBUG

SPIClass spi; // Medio de Comunicacion con el Almacenamiento

//Velocidad del Monitor Serial
#define SERIAL_BAUDRATE 115200
//Frecuencia de Actualizacion del Clock (En Milisegundos)
#define REFRESH_CLOCK 500 
// Retocar macro en caso de querer establecer cuanto se pone el modo apagado luego de estar ese tiempo en el modo SLEEPING (En Segundos)
#define SLEEP_TIME_WAITING_TO_SHUTDOWN (5U * 60U)   // 5 minutos en segundos

// Retocar macro en caso de querer establecer cuanto tiempo el usuario debe mantener el boton back para poner el modo SLEEPING (En Segundos)
#define SLEEP_TIME_BUTTONPRESSING (5U)

#define CLOCK_ON
#define WIFI_ON  
//#define BLUETOOTH_ON

#define SSID_IN         "TP-LINK_4F48"        //SSID del access point en el que se conectara el servicio WiFi
#define PASSWORD_IN     "51807511"            //PASSWORD del access point en el que se conectara el servicio WiFi

//(Desarrollo) Pagina Web
#define SSID_OUT        "Remote_Controller"   //SSID del access point que se generara
#define PASSWORD_OUT    "Authors:GMJ"       //PASSWORD del access point que se generara

void setup(){
    
    Serial.begin(SERIAL_BAUDRATE , SERIAL_8N1 );

    // Configuracion de protocolo de comunicacion para el almacenamiento
    spi=SPIClass(VSPI); 
    spi.begin();
    
    // Inicializacion del sistema del display
    displayBegin();     Serial.println(F("Display Inicializado."));

    // Inicializacion del sistema de botones
    buttonsBegin();     Serial.println(F("Botonera Inicializada."));

    // Inicializacion del sistema de almacenamiento
    SDBegin();          Serial.println(F("Almacenamiento Inicializado."));

    // Inicializacion del sistema del infrarrojo
    infraredBegin();    Serial.println(F("Infrared Inicializado."));
    
    // configSaved(); // (Canceled) De momento cancelado hasta nuevo aviso

    // Espero a que todos los procesos terminen para inicializar
    Serial.flush(); yield();
    /*
    // Task para correr el programa principal
    xTaskCreate(
        Task_Idle,                      // Funcion codigo del Task
        "Task_Idle",                    // Nombre del Task 
        100024U,                        // Reserva de espacio en la Pila
        NULL,                           // Argumentos
        tskIDLE_PRIORITY,               // Prioridad
        &handleIdle                     // Handle 
    );
    */
    // Crear tarea del Watchdog Timer
    xTaskCreate(
        Task_WatchDogTimer,             // Funcion codigo del Task
        "Task_WatchDogTimer",           // Nombre del Task
        configMINIMAL_STACK_SIZE + 256U,// Reserva de espacio en la Pila
        NULL,                           // Argumentos
        tskIDLE_PRIORITY + 2U,          // Prioridad
        NULL                            // Sin Handle
    );

    // Task para preguntar si el usuario esta activo de forma dinamica 
    xTaskCreate(
        Task_AFK,                       //Funcion codigo del Task
        "Task_AFK",                     //Nombre del Task 
        configMINIMAL_STACK_SIZE + 256U,//Reserva de espacio en la Pila 
        NULL,                           //Argumentos
        tskIDLE_PRIORITY + 1U,          //Prioridad
        &handleSleep                    //Handle   
    );

    // Task para preguntar si se desea un reinicio de emergencia
    xTaskCreate(
        Task_emergencyRestart,              //Funcion codigo del Task
        "Task_AskingForEmergencyRestart",   //Nombre del Task 
        configMINIMAL_STACK_SIZE + 256U,    //Reserva de espacio en la Pila 
        NULL,                               //Argumentos 
        tskIDLE_PRIORITY + 2U,              //Prioridad
        NULL                                //Handle   
    );

    UI.show = true;
    UI.show_battery = true;
    UI.show_storage = false;
    UI.run(); // Task para la User Interface

    // Iniciar el scheduler de FreeRTOS
    //vTaskStartScheduler(); // Genera un core dump si se permite el uso del void loop() Asegurarse de usar si es que no se usa el void loop() (Generado por el Watch Dog Timer)

}

//Puesto unicamente por el framework de arduino. El sistema esta montado en Tasks gracias a FreeRTOS
void loop(){ MODE::hub(); } //El Idle esta ubicado en System.h (Programa principal)