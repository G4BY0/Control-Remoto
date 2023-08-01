//Copyright Grupo 11, Inc. All Rights Reserved.
/***********************************************
 * * * * * * * * * * * * * * * * * * * * * * * *
 * \file WifiNetwork
 * This permits user to see all near Network to connect
 *
 * * * * * * * * * * * * * * * * * * * * * * * *
***********************************************/
#ifndef WIFINETWORKS_H
#define WIFINETWORKS_H

#include <cstdint>
#include <vector>
#ifdef ESP32
#include <Wifi.h>
#elif ESP8266
#include <ESP8266WiFi.h>
#endif
#include <Preferences.h>
#include "Interface.h"


static constexpr auto WAITING_TO_CONNECT_MS = 15U * 1000U; // Tiempo de espera en milisegundos

namespace MODE{

  void wifiNetworks(void);

};



#endif // WIFINETWORKS_H