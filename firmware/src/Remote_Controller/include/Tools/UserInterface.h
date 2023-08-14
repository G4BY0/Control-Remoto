//Copyright Grupo 11, Inc. All Rights Reserved.
/***********************************************
 * * * * * * * * * * * * * * * * * * * * * * * *
 * \file
 * UserInterface
 * 
 * * * * * * * * * * * * * * * * * * * * * * * *
***********************************************/
#ifndef USERINTERFACE_H
#define USERINTERFACE_H


#include <cstdio>
#include <string>
#include <sstream>
#include <Arduino.h>
#include <freertos/FreeRTOS.h>  //FreeRTOS Operative System
#include <freeRTOS/task.h>      //MultiThreading
#include <freeRTOS/semphr.h>    //Semaphore

#include "Interface.h"
#include "System.h"
#include "Fonts/icons_UI.hpp"

//! \brief UserInterface
class UI_t{
private:
  enum class UI_position{
  UPPERLEFT_CORNER  = 0,
  UPPERRIGHT_CORNER = 1,
  LOWERLEFT_CORNER  = 2,
  LOWERRIGHT_CORNER = 3
  };
public:
    bool show = false; // If it's true. The User Interface will be shown
    bool show_time = false; // If it's true. The time will be shown
    bool show_battery = false; // If it's true. The baterry percentage will be shown
    bool show_storage = false; // If it's true. The free storage percentage will be shown 
    TaskHandle_t handle; // handle al task asincronico del UI
public:
  /*! \brief Constructor*/
  UI_t(void);
  /*! \brief Destructor*/
  ~UI_t(void); 
  /*! \brief Start Running User Interface
      \note Asincronic Task*/
  void run(void); 
  /*! \brief Stop Running User Interface
      \note (Deleting Task)*/
  void stop(void); // Task Delete
  
  /*! \brief Graphic The UI in the output*/
  void graphic(void);

private:
  /*! \brief Obtiene porcentage total de bateria
      \param buff Buffer
      \note Lo carga el string en el buffer dado*/
  void batteryPercentage(char* buffer);
  struct tm time_RTC;
}; extern UI_t UI; // Global manipulation of UI

static void UI_async(void* nonParameters); // Task Asincronico
#endif // USERINTERFACE_H