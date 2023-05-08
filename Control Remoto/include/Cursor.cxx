//Copyright Grupo 7, Inc. All Rights Reserved.
/***********************************************
 * * * * * * * * * * * * * * * * * * * * * * * *
 * \file
 * Source code of Cursor class form Cursor.h
 * 
 * * * * * * * * * * * * * * * * * * * * * * * *
***********************************************/

#include "Cursor.h"

Cursor::Cursor(const uint8_t AMOUNT_OF_OPTIONS__){ 
  this->AMOUNT_OF_OPTIONS = AMOUNT_OF_OPTIONS__; 
}

Cursor::Cursor(void){}

const uint8_t Cursor::options(void){
  
  uint8_t moving = 0;
  while(INFINITE_LOOPING){

    if(buttonState(PIN::Buttons::DOWN) == TRUE_PULLDOWN) {
      if(moving = AMOUNT_OF_OPTIONS) {
        moving = 1; // HACE OVERFLOW DE OPCIONES
        display.setCursor(LINE_CURSOR_X, LINE_STRING_Y[3]); // POSICION DEL CURSOR UN INSTANTE ANTES
        display.print(" "); // ESCRIBO UN ESPACIO PARA BORRAR EL CURSOR QUE ESTUVO UN INSTANTE ANTES

        display.setCursor(LINE_CURSOR_X, LINE_STRING_Y[moving]); // POSICION DEL CURSOR
        display.print(CURSOR_CHARACTER); // DIBUJO EN PANTALLA EL CURSOR
        display.display(); // EXPULSO A RAM LAS INSTRUCCIONES DEL DISPLAY
      }
      else {
        moving++;
        
        display.setCursor(LINE_CURSOR_X, LINE_STRING_Y[moving-1]); // POSICION DEL CURSOR UN INSTANTE ANTES
        display.print(" "); // ESCRIBO UN ESPACIO PARA BORRAR EL CURSOR QUE ESTUVO UN INSTANTE ANTES

        display.setCursor(LINE_CURSOR_X, LINE_STRING_Y[moving]); // POSICION DEL CURSOR
        display.print(CURSOR_CHARACTER); // DIBUJO EN PANTALLA EL CURSOR
        display.display(); // EXPULSO A RAM LAS INSTRUCCIONES DEL DISPLAY
      }
    }

    else if(buttonState(PIN::Buttons::UP) == TRUE_PULLDOWN) {
      if(moving = AMOUNT_OF_OPTIONS) {
        moving = 1; // HACE OVERFLOW DE OPCIONES
      }
      else {
        moving--;
        display.setCursor(LINE_CURSOR_X, LINE_STRING_Y[moving+1]); // POSICION DEL CURSOR UN INSTANTE ANTES
        display.print(" "); // ESCRIBO UN ESPACIO PARA BORRAR EL CURSOR QUE ESTUVO UN INSTANTE ANTES

        display.setCursor(LINE_CURSOR_X, LINE_STRING_Y[moving]); // POSICION DEL CURSOR
        display.print(CURSOR_CHARACTER); // DIBUJO EN PANTALLA EL CURSOR
        display.display(); // EXPULSO A RAM LAS INSTRUCCIONES DEL DISPLAY
      }
    }

    else if(buttonState(PIN::Buttons::ENTER) == TRUE_PULLDOWN) { return(moving);              }
    else if(buttonState(PIN::Buttons::BACK) == TRUE_PULLDOWN)  { return(BUTTON_PRESSED_BACK); }

  }

}

const char* Cursor::writer_ptr(void){
  /*
  char* pointer;
  while(pointer != '\0') {
     DEVELOPING

    if(buttonState(PIN::Buttons::UP) == TRUE_PULLDOWN) {
      if(moving = AMOUNT_OF_OPTIONS) {
        moving = 1; // HACE OVERFLOW DE OPCIONES
      }
      else {
        moving--;
        display.setCursor(,); // POSICION DEL CURSOR UN INSTANTE ANTES
        display.print(" "); // ESCRIBO UN ESPACIO PARA BORRAR EL CURSOR QUE ESTUVO UN INSTANTE ANTES

        display.setCursor(LINE_CURSOR_X, LINE_STRING_Y[moving]); // POSICION DEL CURSOR
        display.print(CURSOR_CHARACTER); // DIBUJO EN PANTALLA EL CURSOR
        display.display(); // EXPULSO A RAM LAS INSTRUCCIONES DEL DISPLAY
      }
    }

    
    if(buttonState(PIN::Buttons::ENTER) == TRUE_PULLDOWN) { return ("ENTER"); }
    if(buttonState(PIN::Buttons::BACK) == TRUE_PULLDOWN) { return ("BACK"); }
    
  }  
  */  

}

CursorV2::CursorV2(char** menuOptions, Adafruit_SH1106G* display) : options(menuOptions), sh1106(display), currentIndex(0), currentPage(0) {
  totalPages = (getNumberOfOptions() - 1) / 6 + 1; // calcular el número total de páginas
}

int CursorV2::getNumberOfOptions() {
  int count = 0;
  while(options[count] != nullptr) {
    count++;
  }
  return count;
}

void CursorV2::showCurrentPage() {
  sh1106->clearDisplay();
  sh1106->setCursor(0, 0);
  sh1106->print("Seleccione una opcion:");
  for (int i = currentPage * 6; i < getNumberOfOptions() && i < (currentPage + 1) * 6; i++) {
    sh1106->setCursor(0, (i - currentPage * 6 + 1) * 10);
    if (i == currentIndex) {
      sh1106->print(">");
    }
    sh1106->print(options[i]);
  }
  sh1106->display();
}

char* CursorV2::getSelectedOption() {
  showCurrentPage();
  while (true) {
    if (digitalRead(UP_BUTTON_PIN) == LOW) {
      currentIndex--;
      if (currentIndex < 0) {
        currentIndex = getNumberOfOptions() - 1;
      }
      if (currentIndex < currentPage * 6) {
        currentPage--;
        if (currentPage < 0) {
          currentPage = totalPages - 1;
        }
        showCurrentPage();
      } else {
        sh1106->clearDisplay();
        sh1106->setCursor(0, (currentIndex - currentPage * 6 + 1) * 10);
        sh1106->print(">");
        sh1106->print(options[currentIndex]);
        sh1106->display();
      }
      delay(200);
    }
    if (digitalRead(DOWN_BUTTON_PIN) == LOW) {
      currentIndex++;
      if (currentIndex >= getNumberOfOptions()) {
        currentIndex = 0;
      }
      if (currentIndex >= (currentPage + 1) * 6) {
        currentPage++;
        if (currentPage >= totalPages) {
          currentPage = 0;
        }
        showCurrentPage();
      } else {
        sh1106->clearDisplay();
        sh1106->setCursor(0, (currentIndex - currentPage * 6 + 1) * 10);
        sh1106->print(">");
        sh1106->print(options[currentIndex]);
        sh1106->display();
      }
      delay(200);
    }
    if (digitalRead(ENTER_BUTTON_PIN) == LOW) {
      return options[currentIndex];
    }
    if (digitalRead(BACK_BUTTON_PIN) == LOW) {
      return "ReturnedBack";
    }
  }
}

