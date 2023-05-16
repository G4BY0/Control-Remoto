#warning "Project is in developing, it's not already yet!"
#include "Arduino.h"
#include "Interface.h"

namespace MODE{

    void hub__(void);
    void profiles__(void);
    void addProfile__(void);
    void deleteProfile__(void);
    void addSubProfile__(void);
    void deleteSubProfile__(void);
};



void setup(){
    
    Wire.begin();
    
    Serial.begin(9600);
    Serial.println("Tipo de compilador Utilizado: ");

    //Aviso del compilador utilizado (usando los identificadores de cada uno)
    #pragma region Compilador_Usado
    #if defined(__GNUC__)
        Serial.println("GNU :)");
        Serial.print("Version del compilador de GNU es: ");
        Serial.println(__GNUC__);
    #elif defined(__clang__)
        Serial.println("CLANG !!");
        Serial.print("Version Principal del compilador de CLANG es: "); 
        Serial.println(__clang_major__);
        Serial.print("Version Secundaria del compilador de CLANG es: ");
        Serial.println(__clang_minor__);
        Serial.print("Nivel de parche del compilador de CLANG: "); 
        Serial.println(__clang_patchlevel__);
    #else
        Serial.println("Generico :((");
    #endif
    #pragma endregion

    displayBegin();
    buttonsBegin();
    SDBegin();
    
}


//Parte todo del Hub y luego se ramifica en los demas Menus
void loop(){  MODE::hub__(); }



