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

//--------HUB---------------
#define LINE1_X 10
#define LINE1_Y 10
#define LINE2_X 10
#define LINE2_Y 30  
#define LINE3_X 10
#define LINE3_Y 50  
//-------------------------

bool FLAG_CURSOR_UP;
bool FLAG_CURSOR_DOWN;
bool FLAG_CURSOR_LEFT;
bool FLAG_CURSOR_RIGHT;
bool FLAG_CURSOR_BACK;
bool FLAG_CURSOR_ENTER;




inline bool buttonState();
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

/*! @brief Cursor que se desplaza a traves del display

    @param  AMOUNT_OF_OPTIONS
            Cantidad de opciones que puede llegar a desplazarse

*/

class Cursor{

private:

  int AMOUNT_OF_OPTIONS;

public:  

  Cursor(const int AMOUNT_OF_OPTIONS__);

  void Options(void);

};


class Display {

public:
  
  void begin();

  void hub();

};




