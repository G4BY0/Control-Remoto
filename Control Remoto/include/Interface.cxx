#include "Interface.h"


inline bool buttonState(const uint8_t PIN_BUTTON) { 
  return (digitalRead(PIN_BUTTON)); 
}
    
Cursor::Cursor(const int AMOUNT_OF_OPTIONS__){

  this->AMOUNT_OF_OPTIONS = AMOUNT_OF_OPTIONS__;

}

void Cursor::Options(void){

  while(FLAG_CURSOR_ENTER == false){

    if(buttonState(PIN::Buttons::DOWN) == TRUE_PULLDOWN){

      FLAG_CURSOR_DOWN = true;

    }

    else if(buttonState(PIN::Buttons::UP) == TRUE_PULLDOWN){
        
      FLAG_CURSOR_UP = true;

    }

    else if(buttonState(PIN::Buttons::ENTER) == TRUE_PULLDOWN) {
        
      FLAG_CURSOR_ENTER = true;
        
    }
    
  }

}

void Display::begin(void){

  pinMode(PIN::Buttons::UP,     INPUT);
  pinMode(PIN::Buttons::DOWN,   INPUT);
  pinMode(PIN::Buttons::LEFT ,  INPUT);
  pinMode(PIN::Buttons::RIGHT , INPUT);
  pinMode(PIN::Buttons::BACK ,  INPUT);
  pinMode(PIN::Buttons::ENTER,  INPUT);

  FLAG_CURSOR_ENTER = false;
  FLAG_CURSOR_BACK  = false;
  FLAG_CURSOR_UP    = false;
  FLAG_CURSOR_DOWN  = false;
  FLAG_CURSOR_LEFT  = false;
  FLAG_CURSOR_RIGHT = false;

}

void Display::hub(void){
        
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  
  display.setCursor(LINE1_X,LINE1_Y);
  display.print    ("  PROFILES"   );

  display.setCursor(LINE2_X,LINE2_Y);
  display.print    ("  ADD PROFILE");

  display.setCursor(LINE3_X,LINE3_Y);
  display.print    ("  DELETE PROFILE");

  display.display();

  Cursor cursor(3);

  
  

}


