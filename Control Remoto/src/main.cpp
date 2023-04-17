#include "Arduino.h"
#include "Interface.h"

namespace MODE{

    void hub__(void);
    void profiles__(void);
    void addProfile__(void);
    void deleteProfile__(void);
    
};



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



