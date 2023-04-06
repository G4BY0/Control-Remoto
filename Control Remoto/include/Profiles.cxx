#include "Profiles.h"

void Profiles::showProfiles_(void){

    root.openRoot(volume);

    // list all files in the card with date and size
    root.ls(LS_R | LS_DATE | LS_SIZE);


}