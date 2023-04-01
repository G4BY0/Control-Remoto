#include "Arduino.h"
#include "Interface.h"


/**********************************************
void checkFullInitialize_Display();
void checkFullInitialize_SD();

- EN DESARROLLO ESTAS FUNCIONES

*********************************************/

void setup(){
    
    Wire.begin();
    
    Serial.begin(9600);

    displayBegin();

    

    

}


void loop(){

    switch (Interface::hub()){

        case MODE::PROFILES:         Interface::profiles(void);
        case MODE::ADDPROFILE:       Interface::addProfile(void);
        case MODE::DELETEPROFILE:    Interface::deleteProfile(void);

    }



}