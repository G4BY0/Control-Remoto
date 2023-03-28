#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include "PIN.h" 

/* Uncomment the initialize the I2C address , uncomment only one, If you get a totally blank screen try the other*/
#define I2C_ADDRESS     0x3c //initialize with the I2C addr 0x3C Typically eBay OLED's
//#define i2c_Address 0x3d //initialize with the I2C addr 0x3D Typically Adafruit OLED's

#define SCREEN_WIDTH    128 // OLED display width, in pixels
#define SCREEN_HEIGHT   64 // OLED display height, in pixels
#define OLED_RESET      -1   //   QT-PY / XIAO

#define TRUE_PULLUP     0x0;
#define TRUE_PULLDOWN   0x1;
#define FALSE_PULLUP    0x1;
#define FALSE_PULLDOWN  0x0;



const uint8_t LINEX_COMMON = 10; // COORDENADA COMUN DE LINEA EN EL HUB, EJE X

const uint8_t LINEY[3] = {10, 30, 50}; // COORDENADAS DE LINEA EN EL HUB, EJE Y



bool FLAG_CURSOR_UP;    //FLAG PARA SENIALIZAR MOVIMIENTO DEL CURSOR HACIA ARRIBA
bool FLAG_CURSOR_DOWN;  //FLAG PARA SENIALIZAR MOVIMIENTO DEL CURSOR HACIA ABAJO
bool FLAG_CURSOR_LEFT;  //FLAG PARA SENIALIZAR MOVIMIENTO DEL CURSOR HACIA LA IZQUIERDA
bool FLAG_CURSOR_RIGHT; //FLAG PARA SENIALIZAR MOVIMIENTO DEL CURSOR HACIA LA DERECHA
bool FLAG_CURSOR_BACK;  //FLAG PARA SENIALIZAR QUE EL CURSOR SALIO DEL MENU
bool FLAG_CURSOR_ENTER; //FLAG PARA SENIALIZAR QUE EL CURSOR SELECCIONO




inline bool buttonState(const uint8_t PIN_BUTTON);
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

/*! @brief Cursor que se desplaza a traves del display

    @param  AMOUNT_OF_OPTIONS
            Cantidad de opciones que puede llegar a desplazarse

*/
class Cursor{

private:
  const char CURSOR_DRAW = '>';
  uint8_t AMOUNT_OF_OPTIONS;
  uint8_t cordexCursorY(void);
public:  
/*!  @param  AMOUNT_OF_OPTIONS
     Cantidad de opciones que puede llegar a desplazarse
     @param COORDENADA_CURSOR_X
     Es la coordenada en la que el puntero se va a desplazar por el display
     @note Si es para usar como cursor para crear un perfil o subperfil,
     se usa el constructor que no recibe parametros
*/
  Cursor(const uint8_t AMOUNT_OF_OPTIONS__, const uint16_t COORDENADA_CURSOR_X);
  Cursor(void);

  void Options(void);

  void Writer_ptr(void);

};


class Display {

public:
  
  void begin(void);

  void hub(void);

};




