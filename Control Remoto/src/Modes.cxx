
#include "Modes.hpp"


void MODE::hub__(void){

  switch (Interface::hub()){

    case 0:     profiles__();               break;   
    case 1:     addProfile__();             break;
    case 2:     deleteProfile__();          break;
    
    #pragma region Pre-Alpha Modes
    case 3:     addSubProfile__();          break;
    case 4:     deleteSubProfile__();       break;
    #pragma endregion

  }

}

inline void MODE::profiles__(void)        { Interface::profiles(); }

inline void MODE::addProfile__(void)      { Interface::addProfile(); }

inline void MODE::deleteProfile__(void)   { Interface::deleteProfile(); }

inline void MODE::addSubProfile__(void)   { Interface::createSubProfile(); }

inline void MODE::deleteSubProfile__(void){ Interface::deleteSubProfile(); }

#pragma region Developing-Future
inline void MODE::help__(void){ }
#pragma endregion

#pragma region Shutdown
void Shutdown::displayService(void){ }

void Shutdown::SDService(void){ }

void Shutdown::buttonsWaiting(void){ }
#pragma endregion //Shutdown