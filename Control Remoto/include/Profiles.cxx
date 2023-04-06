#include "Profiles.h"

void Profiles::showProfiles_(void){
    
    //FUNCION DEPRECATED, despues ver su nueva funcion!!!!
    root.openRoot(volume);

    // list all files in the card with date and size
    root.ls(LS_R | LS_DATE | LS_SIZE);


}