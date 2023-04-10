#include <IRremote.hpp>


// Storage for the recorded code
struct storedIRDataStruct {
    IRData receivedIRData;
    // extensions for sendRaw
    uint8_t rawCode[RAW_BUFFER_LENGTH]; // The durations if raw
    uint8_t rawCodeLength; // The length of the code
} sStoredIRData;


/*! @brief Inicializacion de infrarrojos
    @note Arduino por defecto establece como entrada los pines digitales*/
void infraredBegin(void);

/*! @brief Preparado para recibir la senial infrarroja
    @note Crea una estructura que almacena cadena binaria (guardada en hexadecimal), numero de bits, etc
    
*/
void infraredReceive(void);

/*! @brief Envia senial infrarroja
    @param profileName nombre del perfil que se encuentra
    @param subProfileName nomber del subperfil que denomina a la cadena binaria
*/
void infraredSend(void);



