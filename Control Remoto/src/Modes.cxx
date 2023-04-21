#include "main.cpp"

void MODE::hub__(void){

  switch (Interface::hub()){

    case MODE_PROFILES:         MODE::profiles__();      
    case MODE_ADDPROFILE:       MODE::addProfile__();    
    case MODE_DELETEPROFILE:    MODE::deleteProfile__(); 

  }

}

void MODE::profiles__(void){

  if(Interface::profiles()){

    


  } 

}

void MODE::addProfile__(void){

  if(Interface::addProfile()){

    Profiles::createProfile_();

  } 

}

void MODE::deleteProfile__(void){

  if(Interface::deleteProfile()){

    Profiles::deleteProfile_();

  } 


}
