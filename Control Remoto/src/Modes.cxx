
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
    case 5: help__();                       break;
    default: break;

  }

}

inline void MODE::profiles__(void)        { Interface::profiles(); }
inline void MODE::addProfile__(void)      { Interface::addProfile(); }
inline void MODE::deleteProfile__(void)   { Interface::deleteProfile(); }
inline void MODE::addSubProfile__(void)   { Interface::createSubProfile(); }
inline void MODE::deleteSubProfile__(void){ Interface::deleteSubProfile(); }

#pragma region Developing-Future
inline void MODE::help__(void){ Interface::help(); /*Apartado de Joaco Para el desarrollo de la pagina*/ }
#pragma endregion //Developing-Future (Opciones adicionales en un futuro)

#pragma region Shutdown
inline void Shutdown::displayService(void){ /*Aun no Desarrollado*/ }
inline void Shutdown::SDService(void){ /*Aun no Desarrollado*/ }
inline void Shutdown::buttonsWaiting(void){ /*Aun no Desarrollado*/ }
#pragma endregion //Shutdown