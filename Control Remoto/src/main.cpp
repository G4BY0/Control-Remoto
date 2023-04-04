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

    hub__(void);

}

void profile__(void){

    if(BUTTON_PRESSED::BACK = Interface::profiles(void)){

        hub__();

    }
    else{ //DEFINITIVAMENTE QUE SI NO ES EQUIVALENTE A BACK, ES ENTER

        /*showPorofiles*/

    }

    

}

void addProfle__(void){



}

void deleteProfile__(void){



}

void hub__(void){

    switch (Interface::hub()){

        case MODE::PROFILES:         Interface::profiles(void);
        case MODE::ADDPROFILE:       Interface::addProfile(void);
        case MODE::DELETEPROFILE:    Interface::deleteProfile(void);

    }

}