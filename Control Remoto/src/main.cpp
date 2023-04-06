#include "Arduino.h"
#include "Interface.h"


/**********************************************
void checkFullInitialize_Display();
void checkFullInitialize_SD();

- EN DESARROLLO ESTAS FUNCIONES

*********************************************/

namespace MODE{

    void hub__(void);
    void profiles__(void);
    void addProfile__(void);
    void deleteProfile__(void);
    
};

void MODE::hub__(void){

    switch (Interface::hub()){

        case MODE_t::PROFILES:         MODE::profiles__();
        case MODE_t::ADDPROFILE:       MODE::addProfile__();
        case MODE_t::DELETEPROFILE:    MODE::deleteProfile__();

    }

}

void MODE::profiles__(void){

    Interface::profiles();

}

void MODE::addProfile__(void){

    Interface::addProfile();

}

void MODE::deleteProfile__(void){

    Interface::deleteProfile();

}


void setup(){
    
    Wire.begin();
    
    Serial.begin(9600);

    displayBegin();

    buttonsBegin();

    SDBegin();
    
}


void loop(){

    MODE::hub__();

}



