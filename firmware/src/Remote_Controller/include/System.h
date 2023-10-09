//Copyright Grupo 11, Inc. All Rights Reserved.
/***********************************************
 * * * * * * * * * * * * * * * * * * * * * * * *
 * \file
 * System (Sistema, Modelacion)
 * 
 * * * * * * * * * * * * * * * * * * * * * * * *
***********************************************/
#ifndef SYSTEM_H
#define SYSTEM_H

#include <cstdio>
#include <Arduino.h>

#include <freertos/FreeRTOS.h>  //FreeRTOS Operative System
#include <freeRTOS/task.h>      //MultiThreading
#include <freeRTOS/semphr.h>    //Semaphore

#include <ESP32Time.h>          //Built-IN RTC
#ifdef ESP32
#include <Wifi.h>
#elif ESP8266
#include <ESP8266WiFi.h>
#endif
#include <WiFiUdp.h>
#include <NTPClient.h>

#include "Tools/UserInterface.h"
#include "Modes/Modes.h"

extern WiFiUDP ntpUDP;
constexpr long utcOffsetInSeconds = -3 * (60 * 60) ; //Argentina == UTF-3
extern NTPClient timeClient;

//Handles
extern TaskHandle_t handleSleep;       //Handle al Task de SLEEPING
extern TaskHandle_t handleIdle;        //Hanlde al Task del idle

//Semaphores
extern SemaphoreHandle_t semaphoreDisplay; //Semaforo para el libre uso del display

#define WIFI_SERVICE_STATUS __wifi // Flag that senialize if the WiFi Service is working or not. 'True' Enabled
#define BLUETOOTH_SERVICE_STATUS __bluetooth // Flag that senialize if the Bluetooth Service is working or not. 'True' Enabled
extern bool __wifi; // Flag that senialize if the WiFi Service is working or not. 'True' Enabled
extern bool __bluetooth; // Flag that senialize if the Bluetooth Service is working or not. 'True' Enabled


//Pulsadores
/*! \brief Estado logico de la botonera
  \param PIN_BUTTON PIN de la placa conectada al boton
  \note Instalacion PullUP
  \returns Si esta presionado 'true', sino 'false' */
#define buttonState(PIN_BUTTON) !digitalRead(PIN_BUTTON) 

//Parte todo del Hub y luego se ramifica en los demas Menus
void Task_Idle(void * __nonParameter);

//PRIVILEGIADO
#define WATCHDOG_FEED_PERIOD_MS (1000U)  // Realimentación del WDT cada 1 segundo
/*! \brief Realimentacion Constante para el Watch Dog Timer
    \note (BUG) NECESARIO PARA QUE EL PROGRAMA NO CRASHEE*/
void Task_WatchDogTimer(void* __nonparameter);

/*! \brief Deja el dispositivo en espera a respuesta del usuario.
    \param Seconds si pasa el tiempo estipulado, se coloca el dispositivo en MODE::ShutDown */
void __sleep(uint32_t Seconds);
#define SLEEP_TIME_WAITING_TO_SHUTDOWN  (5U * 60U)   // Tiempo en cuanto se pone el modo apagado luego de estar ese tiempo en el modo SLEEPING (En minutos)
#define SLEEP_TIME_BUTTONPRESSING       (5U)         // Tiempo en cuanto el usuario debe mantener el boton back para poner el modo SLEEPING (En segundos)
#define MAX_TIME_AFK                    (70U)        // Tiempo maximo de espera en cuanto el usuario esta AFK (En segundos)
/*! \brief Task para el sleeping Mode
    \note El boton BACK se debe mantener presionado en SLEEP_TIME_BUTTONPRESSING O En Caso de no presionar ningun boton en */
void Task_AFK(void* __nonParameter);

//Funcion de ejecucion en el momento de reinicio, no retorna
void restart_now(void);

/*! \brief Stoppea todos los servicios y pone en modo sleeping a la placa de desarrollo */
void ShutDown_now(void);

/*! \brief Manejo de Servicios poniendo en apagado*/
namespace ShutDown{
    
    /*! \brief Detiene el servicio visual */
    void displayService(void);
    /*! \brief Detiene el servicio de almacenamiento */
    void SDService(void);
    /*! \brief Pone en espera los botones hasta alguna respuesta */
    void buttonsWaiting(void);

};

//Handles
extern TaskHandle_t handleBattery;     //Handle al Task de mostrar la bateria
extern TaskHandle_t handleClock;       //Handle al Task de Clock
extern TaskHandle_t handleWiFi;        //Handle al Servicio Wifi
extern TaskHandle_t handleBluetooth;   //Handle al Servicio Bluetooth

/*! \brief Task para mostrar de manera dinamica la bateria
    \note lo muestra en esquinas de la pantalla */
void Task_Battery(void* __nonParameter);

extern ESP32Time RTC; // Objeto global para el libre manejo de el RTC
extern bool __clock; // Flag that senialize if the Clock Service is working or not. 'True' Enabled
#define CLOCK_SERVICE_STATUS __clock
extern bool __clock_analog; // Flag that senialize if the Clock Digital is working or not. 'True' Enabled
#define CLOCK_SERVICE_ANALOG __clock_analog
extern bool __clock_digital;
#define CLOCK_SERVICE_DIGITAL __clock_digital


//Frecuencia de Actualizacion del Clock (En milisegundos)
/*! \brief Muestra el clock en pantalla
    \note Cada Vez que se refresca en pantalla reserva su semaforo*/
void clockBegin(void);

#endif // SYSTEM_H