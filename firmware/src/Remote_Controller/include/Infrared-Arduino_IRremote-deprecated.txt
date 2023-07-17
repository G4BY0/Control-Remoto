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
#include <stdint.h>
#include <memory>
#include "PIN.h"

#define IR_RECEIVE_PIN  PIN::InfraredReceiver::DATA
#define IR_SEND_PIN     PIN::InfraredTransmitter::DATA

#ifndef SUCCESS
  #define SUCCESS EXIT_SUCCESS
#endif
#ifndef FAILURE
  #define FAILURE EXIT_FAILURE
#endif

// Storage for the recorded code
//Estructura de almacenamiento de datos del infrarrojo (sin el nombre del subperfil)
struct storedIRDataStruct;


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
void sendCode(std::shared_ptr<storedIRDataStruct> aIRDataToSend);

/*! @brief Hace una copia de datos del infrarrojo usando lo que tiene guardado la instancia IrReceiver
    @returns Retorna puntero inteligente de accesso unico del objeto de la informacion recibida
    @note Retorna la copia de datos
*/
storedIRDataStruct storeCode(const char* subprofileName);

//DEBUGGING!!!!!!
void julio();
//------------------

#endif //Infrared_h