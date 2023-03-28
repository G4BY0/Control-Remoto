#include "Interface.h"


inline bool buttonState(const uint8_t PIN_BUTTON) { 
  return (digitalRead(PIN_BUTTON)); 
}

uint8_t Cursor::cordexCursorY(void){                                

  if(FLAG_CURSOR_DOWN == true){

    FLAG_CURSOR_DOWN == false;
    


  }

}

Cursor::Cursor(const uint8_t AMOUNT_OF_OPTIONS__, const uint16_t COORDENADA_CURSOR_X){

  this->AMOUNT_OF_OPTIONS = AMOUNT_OF_OPTIONS__;

}


void Cursor::Options(void){
  
  int moving = 0;
  while(FLAG_CURSOR_ENTER == false){

    if(buttonState(PIN::Buttons::DOWN) == TRUE_PULLDOWN){
      if(moving = AMOUNT_OF_OPTIONS)
        moving = 0;
      else
        moving++;
      FLAG_CURSOR_DOWN = true;
      
    }

    else if(buttonState(PIN::Buttons::UP) == TRUE_PULLDOWN){
      if(moving = 0)
        moving = AMOUNT_OF_OPTIONS;
      else
        moving--;
      FLAG_CURSOR_UP = true;

    }

    else if(buttonState(PIN::Buttons::ENTER) == TRUE_PULLDOWN) {
        
      FLAG_CURSOR_ENTER = true;
        
    }
    
    display.setCursor(LINEX_COMMON, LINEY[moving]);
    display.print(CURSOR_DRAW);
    display.fillRect();
  }

}

void Cursor::Writer_ptr(void){




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
  
  display.setCursor(LINEX_COMMON,LINEY[0]);
  display.print    ("  PROFILES"   );

  display.setCursor(LINEX_COMMON,LINEY[1]);
  display.print    ("  ADD PROFILE");

  display.setCursor(LINEX_COMMON,LINEY[2]);
  display.print    ("  DELETE PROFILE");

  display.display();

  Cursor cursor(3);

  
  

}


