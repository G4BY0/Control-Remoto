//Copyright Grupo 7, Inc. All Rights Reserved.
/***********************************************
 * * * * * * * * * * * * * * * * * * * * * * * *
 * \file
 * Definicion de metodos de las Clases: Cursor
 *                                      CursorV2
 *                                      Writter
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
  while (true) {
    showCurrentPage();
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

Writter::Writter(Adafruit_SH1106G& object) : display(object){}

void Writter::Graphics(void){
  // Clear the buffer
  display.clearDisplay();
  display.display();
  
  // Display filled in rect in the top section of the display when To_Transfer would be output
  display.fillRect(0, 0, 128, 15, SH110X_INVERSE);
  display.drawRect(110, 2, 16, 12, SH110X_BLACK);
  display.setTextSize(1);            
  display.setTextColor(SH110X_BLACK);  
  display.setCursor(113,4); 
  display.println("OK");
  display.display();
  // Display Letter Board 3 rows 9 character in each row 
  display.setTextSize(2);            
  display.setTextColor(SH110X_WHITE);       
  for (int j=0; j<3;j++){
    for (int i=0; i<9;i++){
       display.setCursor(i*12+2*i+1,j*16+17);           
       display.println(Letters[j][i]);
       delay(10);
     display.display();
    }
  }
  // Highlight character A by displaying Inverse rect at first position
  display.fillRect(0, 16, 12, 16, SH110X_INVERSE);
  display.display();
}

void Writter::Highlight_letter(int X, int X_Old, int Y, int Y_Old){
  // When position changes 
  // Draw the inverse rect in the Old_pos to deactivate  the highlight in the old spot
  // Draw the inverse rect to Highlite the new spot
  // Displaying Inverse rect in a new position to highlight
  display.fillRect(X*12+2*X, Y*16 +16, 12, 16, SH110X_INVERSE);
  // Displaying Inverse rect in the old positon to unhighlight
  display.fillRect(X_Old*12+2*X_Old, Y_Old*16 +16, 12, 16, SH110X_INVERSE);
  display.display();
}

void Writter::keyboard(void) {
  Key_read =analogRead(Keyboard);
  
  Key_pressed=true;
  if ((buttonState(LEFT_BUTTON_PIN) == LOW) and Old_X>0){ New_X=Old_X-1; delay(300);}
  if ((buttonState(RIGHT_BUTTON_PIN) == LOW) and Old_X<9){ New_X=Old_X+1; delay(300);}
  if ((buttonState(UP_BUTTON_PIN) == LOW) and Old_Y>-1){ New_Y=Old_Y-1; delay(300);}
  if ((buttonState(DOWN_BUTTON_PIN) == LOW) and  Old_Y<2 ){ New_Y=Old_Y+1; delay(300);}
  if (buttonState(RIGHT_BUTTON_PIN) == LOW) {
  delay(300);
   if (New_Y!=-1){
     To_Transmit=To_Transmit + Letters[New_Y][New_X];
     To_Transmit_Length++;
     display.setTextSize(1);
     display.setCursor(3,1);
     display.setTextColor(SH110X_BLACK);
     display.fillRect(0, 0, 100, 15, SH110X_WHITE);
     display.println(To_Transmit);
     display.display();
   }
   else{
    for (int i=1;i<9;i++) {
      display.fillRect(0, 0, 128, 15, SH110X_INVERSE);
      delay(300);
      display.display();
    }
   }
  }     
  if (New_Y==-1 and Old_Y==0){
      display.fillRect(110, 2, 16, 12, SH110X_INVERSE); 
      display.fillRect(Old_X*12+2*Old_X, Old_Y*16 +16, 12, 16, SH110X_INVERSE); 
  }
  if (New_Y==0 and Old_Y==-1){
      display.fillRect(110, 2, 16, 12, SH110X_INVERSE); 
      display.fillRect(New_X*12+2*New_X, New_Y*16 +16, 12, 16, SH110X_INVERSE);
      Prev_Key_read=Key_read;
      Old_X=New_X;
      Old_Y=New_Y;;
  }
  if ((Old_X!=New_X or Old_Y!=New_Y) and Old_Y!=-1 ){
      if (New_Y!=-1 )Highlight_letter (New_X,Old_X,New_Y,Old_Y);
      Old_X=New_X;
      Old_Y=New_Y;
  }
  
  display.display();
  Prev_Key_read=Key_read; 

}

inline String Writter::stringFinished(void){ return To_Transmit; } // Desarrollar para el loop
