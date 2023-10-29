//Copyright Grupo 11, Inc. All Rights Reserved.
/***********************************************
 * * * * * * * * * * * * * * * * * * * * * * * *
 * \file
 * Manejo de las señales infrarrojas 
 * 
 * * * * * * * * * * * * * * * * * * * * * * * *
***********************************************/
#ifndef INFRARED_H
#define INFRARED_H

#include <Arduino.h>
#include <cstdint>
#include <memory>
#include <algorithm>
#include "PIN.h"
#include <IRsend.h>
#include <IRrecv.h>
#include <IRremoteESP8266.h> // Tambien tiene soporte para placas espressif32!
#include <IRutils.h>

#include "PIN.h"

// The GPIO an IR detector/demodulator is connected to. Recommended: 14 (D5)
// Note: GPIO 16 won't work on the ESP8266 as it does not have interrupts.
// Note: GPIO 14 won't work on the ESP32-C3 as it causes the board to reboot.
constexpr uint16_t kRecvPin = PIN::InfraredReceiver::DATA;
// GPIO to use to control the IR LED circuit. Recommended: 4 (D2).
constexpr uint16_t kIrLedPin = PIN::InfraredTransmitter::DATA;
// The Serial connection baud rate.
// NOTE: Make sure you set your Serial Monitor to the same speed.
constexpr uint32_t kBaudRate = 115200;
// As this program is a special purpose capture/resender, let's use a larger
// than expected buffer so we can handle very large IR messages.
// i.e. Up to 512 bits.
constexpr uint16_t kCaptureBufferSize = 1024U;
// kTimeout is the Nr. of milli-Seconds of no-more-data before we consider a
// message ended.
constexpr uint8_t kTimeout = 50U;  // Milli-Seconds
// kFrequency is the modulation frequency all messages will be replayed at.
constexpr uint16_t kFrequency = 38000U;  // in Hz. e.g. 38kHz.

// Storage for the recorded code
// Estructura de almacenamiento de datos del infrarrojo
struct storedIRDataStruct {
  decode_results results;
  char nameSubProfile[30] = {'\0'}; // Nombre del subperfil + Inicializacion
};

/*! @brief Inicializacion de infrarrojos
    @note Arduino por defecto establece como entrada los pines digitales*/
void infraredBegin(void);

/*! @brief Inicia el proceso para recibir señales infrarrojas
    @note Crea una estructura que almacena cadena binaria (guardada en hexadecimal), numero de bits, etc  
*/
void Receive_start(void);

/*! @brief Checkea si es correcta la cadena binaria recibida
    @note Compara con la escructura del objeto IrReceiver.decodedIRData
    @returns Si pasa las pruebas retorna EXIT_SUCCESS, sino EXIT_FAILURE.
*/
bool Receive_check(void);

/*! @brief Checkea si es correcta la cadena binaria recibida
    @note Compara con la escructura del objeto IrReceiver.decodedIRData
*/
void Receive_stop(void);

/*! @brief Envia señal infrarroja
    @param aIRDataToSend recibe estructura con la data de la señal
    @note usara la estructura recibida referencia para el envio de la informacion
*/
void sendCode(std::shared_ptr<storedIRDataStruct> IRData);

/*! @brief Hace una copia de datos del infrarrojo usando lo que tiene guardado la instancia IrReceiver
    @returns Retorna puntero inteligente de accesso unico del objeto de la informacion recibida
    @note Retorna la copia de datos
*/
std::shared_ptr<storedIRDataStruct> storeCode( const char* subprofileName );

#endif //Infrared_h