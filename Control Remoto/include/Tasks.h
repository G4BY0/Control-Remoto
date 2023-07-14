//Copyright Grupo 11, Inc. All Rights Reserved.
/***********************************************
 * * * * * * * * * * * * * * * * * * * * * * * *
 * 
 * Tasks for the Program
 * 
 * * * * * * * * * * * * * * * * * * * * * * * *
***********************************************/

#ifndef TASKS_H
#define TASKS_H

#include <freertos/FreeRTOS.h>  //FreeRTOS Operative System
#include <freeRTOS/task.h>      //MultiThreading
#include <freeRTOS/semphr.h>    //Semaphore
#include <WiFi.h>               //Wi-Fi
#include <WiFiUdp.h>
#include <ESP32Time.h>          //Built-IN RTC
#include "Modes.h"

extern TaskHandle_t handleBattery;     //Handle al Task de mostrar la bateria
extern TaskHandle_t handleSleep;       //Handle al Task de SLEEPING
extern TaskHandle_t handleIdle;        //Hanlde al Task del idle
extern TaskHandle_t handleClock;       //Handle al Task de Clock
extern TaskHandle_t handleWiFi;        //Handle al Servicio Wifi
extern TaskHandle_t handleBluetooth;   //Handle al Servicio Bluetooth

/*! \brief Task para mostrar de manera dinamica la bateria
    \note lo muestra en esquinas de la pantalla */
void Task_Battery(void* __nonParameter);

#define SLEEP_TIME_WAITING_TO_SHUTDOWN  (5U * 60U)   // Tiempo en cuanto se pone el modo apagado luego de estar ese tiempo en el modo SLEEPING (En minutos)
#define SLEEP_TIME_BUTTONPRESSING       (5U)         // Tiempo en cuanto el usuario debe mantener el boton back para poner el modo SLEEPING (En segundos)
#define SLEEP_TIME_AFK                  (70U)        // Tiempo maximo de espera en cuanto el usuario esta AFK (En segundos)
/*! \brief Task para el sleeping Mode
    \note El boton BACK se debe mantener presionado en SLEEP_TIME_BUTTONPRESSING O En Caso de no presionar ningun boton en */
void Task_Sleep(void* __nonParameter);

//Tarea de ejecucion en el momento de reinicio
void Task_Restart(void* __nonParameter);

//Parte todo del Hub y luego se ramifica en los demas Menus
void Task_Idle(void * __nonParameter);

#define WATCHDOG_FEED_PERIOD_MS (1000U)  // Realimentación del WDT cada 1 segundo
/*! \brief Realimentacion Constante para el Watch Dog Timer
    \note (BUG) NECESARIO PARA QUE EL PROGRAMA NO CRASHEE*/
void Task_WatchDogTimer(void* __nonparameter);

extern ESP32Time RTC;
//Frecuencia de Actualizacion del Clock (En milisegundos)
#define REFRESH_CLOCK 500
/*! \brief Muestra el clock en pantalla
    \param nonParameter Sin Parametros
    \note Cada Vez que se refresca en pantalla reserva su semaforo*/
void Task_Clock(void* __nonParameter);

extern bool __Wifi; // Flag that senialize if the WiFi Service is working or not. 'True' Enabled

/*! \brief Servicio Wifi*/
void Task_Wifi(void* nonParameter);

/*! \brief Servicio Bluetooth*/
void Task_Bluetooth(void* nonParameter);

#endif // TASKS_H