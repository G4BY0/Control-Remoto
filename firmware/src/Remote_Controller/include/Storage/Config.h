//Copyright Grupo 11, Inc. All Rights Reserved.
/***********************************************
 * * * * * * * * * * * * * * * * * * * * * * * *
 * 
 * This manages the saved config from config.txt
 * Thanks for stevemarple (GITHUB) for the library IniFile
 *
 * * * * * * * * * * * * * * * * * * * * * * * *
***********************************************/
#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <IniFile.h>
#include <Preferences.h>

#include "System.h" //Where the save configuration will be stored if it success the proccess (CLOCK_SERVICE_STATUS, WIFI_SERVICE_STATUS, BLUETOOTH_SERVICE_STATUS)
#include "Modes/Clock.h" //Where the save configuration will be stored if it success the proccess (CLOCK_DIGITAL_STATUS, CLOCK_ANALOG_STATUS)

// Path del archivo que se usara para poder sobreescribir perfiles
#define CONFIG_FILE_DIRANDNAME    "/config.db"
#define CONFIG_FILE_NAME          "config"

#define SUCCEDED 1
#define FAILED 0
//config.ini file object
extern IniFile config;

/*! \brief Settings up saved config
    \returns if returns 'false' it failed, else 'true'*/
bool configSaved(void);


/*! \brief Saved Config from internal Storage*/
static bool configSaved_internalStorage(void);

/*! \brief Saved Config from External Storage*/
static bool configSaved_externStorage(void);

/*! \brief Just print with an enum of the library IniFile.h
    \param enum enumeral
    \param eol */
static void printErrorMessage(uint8_t e, bool eol = true);

#endif //CONFIG_H