
#include "Modes.h"


void MODE::hub__(void){

  switch (Interface::hub()){

    case 0:     profiles__();               break;   
    case 1:     addProfile__();             break;
    case 2:     deleteProfile__();          break;
    
    #pragma region Pre-Alpha Modes
    case 3:     addSubProfile__();          break;
    case 4:     deleteSubProfile__();       break;
    #pragma endregion
    default: break;

  }

}

void MODE::profiles__(void)        { Interface::profiles(); }

void MODE::addProfile__(void)      { Interface::addProfile(); }

void MODE::deleteProfile__(void)   { Interface::deleteProfile(); }

void MODE::addSubProfile__(void)   { Interface::createSubProfile(); }

void MODE::deleteSubProfile__(void){ Interface::deleteSubProfile(); }

#pragma region Developing-Future
void MODE::help__(void){ /*Apartado de Joaco*/ }
#pragma endregion //Developing-Future (Opciones adicionales en un futuro)

#pragma region Shutdown
void Shutdown::displayService(void){ }

void Shutdown::SDService(void){ }

void Shutdown::buttonsWaiting(void){ }
#pragma endregion //Shutdown