//Copyright Grupo 7, Inc. All Rights Reserved.
/***********************************************
 * * * * * * * * * * * * * * * * * * * * * * * *
 * \file
 * Manejo de las señales infrarrojas 
 * 
 * * * * * * * * * * * * * * * * * * * * * * * *
***********************************************/

#include <Arduino.h>
#include <IRremote.hpp>
#include "PIN.h"

#ifndef INFRARED_H
#define INFRARED_H

#ifndef SUCCESS
  #define SUCCESS EXIT_SUCCESS
#endif
#ifndef FAILURE
  #define FAILURE EXIT_FAILURE
#endif


#define IR_RECEIVE_PIN  PIN::InfraredReceiver::DATA
#define IR_SEND_PIN     PIN::InfraredTransmitter::DATA

#ifndef storedIRDataStruct_type
#define storedIRDataStruct_type
// Storage for the recorded code
struct storedIRDataStruct {
    IRData receivedIRData;
    // extensions for sendRaw
    uint8_t rawCode[RAW_BUFFER_LENGTH]; // The durations if raw
    uint8_t rawCodeLength; // The length of the code
};
#endif


/*! @brief Inicializacion de infrarrojos
    @note Arduino por defecto establece como entrada los pines digitales*/
void infraredBegin(void);

/*! @brief Inicia el proceso para recibir señales infrarrojas
    @note Crea una estructura que almacena cadena binaria (guardada en hexadecimal), numero de bits, etc  
*/
inline void Receive_start(void);

/*! @brief Checkea si es correcta la cadena binaria recibida
    @note Compara con la escructura del objeto IrReceiver.decodedIRData
    @returns Si pasa las pruebas retorna EXIT_SUCCESS, sino EXIT_FAILURE.
*/
bool Receive_check(void);

/*! @brief Checkea si es correcta la cadena binaria recibida
    @note Compara con la escructura del objeto IrReceiver.decodedIRData
*/
inline void Receive_stop(void);

/*! @brief Envia señal infrarroja
    @param aIRDataToSend recibe estructura con la data de la señal
    @note usara la estructura recibida referencia para el envio de la informacion
*/
void sendCode(void);

/*! @brief Almacena datos del infrarrojo usando lo que tiene guardado la instancia IrReceiver
    @param profileName nombre del perfil en el que se encontrara el subperfil
    @param subProfileName nombre del subperfil que representara la DATA de la señal infrarroja
    @note usara la estructura recibida para almacenarla en la SD como subperfil de perfil anteriormente creado
*/
void storeCode(const char* profileName, const char* subProfileName);

/*! @brief Conjunto de metodos de preparacion para recibir Infrarrojo
    @returns Infrarrojo Recibido
    @note usara la estructura recibida para almacenarla en la SD como subperfil de perfil anteriormente creado
*/
Keep_t& ReceivingAndStoring(const char* profileName, const char* subProfileName);

#endif //Infrared_h