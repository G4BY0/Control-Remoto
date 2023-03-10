#include "gandulfo.h"


namespace displayOutput{
  
  void Initializing{

    begin();

  }

  class Hub{
    
    private:
    
    const int sizeOfLetters = 3;
    
    Adafruit_ILI9341 *outputTft;

    public:

    void Hub(Adafruit_ILI9341 outputTft_){

      outputTft -> outputTft_;

      outputTft.begin();

    }

    void showProfiles(){

    

    }
    void addProfiles(){


    }
    void deleteProfiles(){


    }
    

  };

};
