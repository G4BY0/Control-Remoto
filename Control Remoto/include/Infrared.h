//Copyright Grupo 7, Inc. All Rights Reserved.
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
#ifndef storedIRDataStruct_type_declaration
#define storedIRDataStruct_type_declaration
struct storedIRDataStruct;
#endif

//Es el equivalente de "struct storedIRDataStruct" + nombre del subperfil {char[20]}
#ifndef Keep_type_declaration
#define Keep_type_declaration
struct Keep_t;
#endif

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
void sendCode(storedIRDataStruct* aIRDataToSend);

/*! @brief Hace una copia de datos del infrarrojo usando lo que tiene guardado la instancia IrReceiver
    @returns Retorna la estructura normalizada de la copia de datos
    @note Retorna la copia de datos almacenado en un espacio de la memoria Heap (tipo estatico)
*/
storedIRDataStruct* storeCode(void);

#pragma region Developing
/*! @brief Conjunto de metodos de preparacion para recibir Infrarrojo
    @param profileName Nombre del perfil dado   
    @param subProfileName Nombre del subperfil dado
    @returns Infrarrojo Recibido
    @note Usara la estructura recibida para almacenarla en la SD como subperfil de perfil anteriormente creado
*/
//void ReceivingAndStoring(const char* profileName, const char* subProfileName);
#pragma endregion

#endif //Infrared_h