#include "main.cpp"

void MODE::hub__(void){

  switch (Interface::hub()){

    case 0:     MODE::profiles__();         break;   
    case 1:     MODE::addProfile__();       break;
    case 2:     MODE::deleteProfile__();    break;
    #pragma region Pre-Alpha Modes
    case 3:     MODE::addSubProfile__();    break;
    case 4:     MODE::deleteSubProfile__(); break;
    #pragma endregion

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

void MODE::addSubProfile__(void){

  Interface::createSubProfile();

}

void MODE::deleteSubProfile__(void){

  Interface::deleteSubProfile();

}