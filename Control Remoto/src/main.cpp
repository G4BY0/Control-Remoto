#include "Interface.h"


/**********************************************
void checkFullInitialize_Display();
void checkFullInitialize_SD();

- EN DESARROLLO ESTAS FUNCIONES

*********************************************/

void setup(){
    
    Wire.begin();
    
    Serial.begin(9600);

    display.begin(I2C_ADDRESS, true);
    display.display();
    display.clearDisplay();

    

}


void loop(){





}