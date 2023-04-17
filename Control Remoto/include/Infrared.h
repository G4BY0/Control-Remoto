#include <Arduino.h>
#include <IRremote.hpp>
#include "PIN.h"

#ifndef INFRARED_H
#define INFRARED_H

// Storage for the recorded code
struct storedIRDataStruct {
    IRData receivedIRData;
    // extensions for sendRaw
    uint8_t rawCode[RAW_BUFFER_LENGTH]; // The durations if raw
    uint8_t rawCodeLength; // The length of the code
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
*/
void Receive_check(void);


/*! @brief Checkea si es correcta la cadena binaria recibida
    @note Compara con la escructura del objeto IrReceiver.decodedIRData
*/
void Receive_stop(void);

/*! @brief Envia señal infrarroja
    @param aIRDataToSend recibe estructura con la data de la señal
    @note usara la estructura recibida referencia para el envio de la informacion
*/
void sendCode(void);

/*! @brief Almacena datos del infrarrojo
    @param sStoredIRData recibe estructura con la data de la señal
    @note usara la estructura recibida para almacenarla en la SD como subperfil de perfil anteriormente creado
*/
void storeCode(storedIRDataStruct* sStoredIRData);


#endif //Infrared_h