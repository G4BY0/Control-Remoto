#include <IRremote.hpp>

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



