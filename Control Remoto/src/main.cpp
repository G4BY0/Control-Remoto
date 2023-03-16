#include "Interface.h"

Adafruit_SSD1306 AdafruitOled_Object(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Display oled(AdafruitOled_Object);

/**********************************************
void checkFullInitialize_Display();
void checkFullInitialize_SD();

- EN DESARROLLO ESTAS FUNCIONES

*********************************************/

void setup(){
    Wire.begin();
    Serial.begin(9600);

    if()

}


void loop(){





}