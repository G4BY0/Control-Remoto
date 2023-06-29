//Copyright Grupo 11, Inc. All Rights Reserved.
/***********************************************
 * * * * * * * * * * * * * * * * * * * * * * * *
 * \file
 * Writter (escritor de strings)
 * usa los botones como perisfericos
 * Permiten: Mayusculas, Minusculas y signos (EL ESPACIO SE MUESTRA ASI -> ' ' Y EN REALIDAD SE ALMACENA COMO '_')
 * 
 * * * * * * * * * * * * * * * * * * * * * * * *
***********************************************/
#include "Cursor.h"

WritterV2::WritterV2(Adafruit_SH1106G* displayReference) : display_ptr(displayReference) { }

///////////////////////////
//      -FUNCTIONS-      //
///////////////////////////

void WritterV2::atcLoop(void) {
  while( booleanStringFinished == false ){
    display.clearDisplay();
    timeDelay();
    drawMenu();
    drawCursor();
    drawText();
    drawBlink();
    display.display();
  }
}//atcLoop

void WritterV2::timeDelay(void) {
  if (buttonDelay <= 3) {
    buttonDelay++;
  }
  if (selectDelay <= 3) {
    selectDelay++;
  }
  if (radioDelay <= 20) {
    radioDelay++;
  }
  if (blinkDelay <= 20) {
    blinkDelay++;
  }
}//timeDelay

void WritterV2::drawMenu(void) {
  if (show == true) {
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);
    display.drawLine(0, 31, 105, 31, SH110X_WHITE);//(x1, y1, x2, y2) horizontal
    display.drawLine(105, 21, 105, 64, SH110X_WHITE);//(x1, y1, x2, y2) verticle
    display.drawLine(0, 21, 128, 21, SH110X_WHITE);//(x1, y1, x2, y2) horizontal
    display.setCursor(109, 25);
    display.println(F("snd"));
    display.setCursor(109, 35);
    display.println(F("ext"));
    display.setCursor(109, 45);
    display.println(F("cap"));
    display.setCursor(109, 55);
    display.println(F("del"));
    if (letters == true && capitalize == false) {
      display.setCursor(1, 35);
      display.println(F("a b c d e f g h i"));
      display.setCursor(1, 45);
      display.println(F("j k l m n o p q r"));
      display.setCursor(1, 55);
      display.println(F("s t u v w x y z _"));
    }
    if (letters == true && capitalize == true) {
      display.setCursor(1, 35);
      display.println(F("A B C D E F G H I"));
      display.setCursor(1, 45);
      display.println(F("J K L M N O P Q R"));
      display.setCursor(1, 55);
      display.println(F("S T U V W X Y Z _"));
    }
    if (letters == false) {
      display.setCursor(1, 35);
      display.println(F("1 2 3 4 5 6 7 8 9"));
      display.setCursor(1, 45);
      display.println(F("0 # & % + - = / *"));
      display.setCursor(1, 55);
      display.println(F(". , ? ! ' : ; ( )"));
    }
  }
}//drawMenu

void WritterV2::drawCursor(void) {
  if (buttonDelay >= 3 && show == true) {
    if(buttonState(PIN::Buttons::BACK) == HIGH){
    msgToSend[0] = '\0';
    index = 0;
    num = 0;
    buttonDelay = 0;
    booleanStringFinished = true;
    return;
    }
    if(buttonState(PIN::Buttons::UP) == HIGH) {
      y -= 10;
      buttonDelay = 0;
      transmit = false;
    }
    if(buttonState(PIN::Buttons::DOWN) == HIGH) {
      y += 10;
      buttonDelay = 0;
      transmit = false;
    }
    if(buttonState(PIN::Buttons::LEFT) == HIGH) {
      x -= 12;
      buttonDelay = 0;
      transmit = false;
    }
    if(buttonState(PIN::Buttons::RIGHT) == HIGH) {
      x += 12;
      buttonDelay = 0;
      transmit = false;
    }
    //limit cursor movement
    if (x < 0) {
      x = 108;
    }
    if (x > 108) {
      x = 0;
    }
    if (x == 108) {
      if (y < 23) {
        y = 53;
      }
      if (y > 53) {
        y = 23;
      }
    }
    else {
      if (y < 33) {
        y = 53;
      }
      if (y > 53) {
        y = 33;
      }
    }
    if (y == 23) {
      x = 108;
    }
    //change rectangle size based on location
    if (x == 108) {
      width = 20;
    }
    else {
      width = 7;
    }
  }
  if (selectDelay >= 3 && buttonState(PIN::Buttons::ENTER) == HIGH) {
    selectDelay = 0;
    select = true;
    
  }
  //draw rectangle
  if (show == true) {
    display.drawRect(x, y, width, height, SH110X_WHITE);
  }
}//drawCursor

void WritterV2::drawText(void) {
  if (show == true) {
    if (capitalize == false && letters == true) {
      
      switch (y) {
        //Si se presiona la opcion "snd"
        case 23:
          if (select == true) {
            select = false;
            transmit = true;
            num++;
            Serial.println(F("snd was pressed... \n String Finished!"));
            booleanStringFinished = true;
            return;

          }
        case 33:
          switch (x) {
            case 0:
              if (select == true) {
                select = false;
                msgToSend[index] = 'a';
                index++;
              }
            break;
            case 12:
              if (select == true) {
                select = false;
                msgToSend[index] = 'b';
                index++;
              }
            break;
            case 24:
              if (select == true) {
                select = false;
                msgToSend[index] = 'c';
                index++;
              }
            break;
            case 36:
              if (select == true) {
                select = false;
                msgToSend[index] = 'd';
                index++;
              }
            break;
            case 48:
              if (select == true) {
                select = false;
                msgToSend[index] = 'e';
                index++;
              }
            break;
            case 60:
              if (select == true) {
                select = false;
                msgToSend[index] = 'f';
                index++;
              }
            break;
            case 72:
              if (select == true) {
                select = false;
                msgToSend[index] = 'g';
                index++;
              }
            break;
            case 84:
              if (select == true) {
                select = false;
                msgToSend[index] = 'h';
                index++;
              }
            break;
            case 96:
              if (select == true) {
                select = false;
                msgToSend[index] = 'i';
                index++;
              }
            break;
            case 108:
              if (select == true) {
                select = false;
                letters = false;
                
              }
            break;
          }
        break;
  ///////////////////////////////////////
        case 43:
          switch (x) {
            case 0:
              if (select == true) {
                select = false;
                msgToSend[index] = 'j';
                index++;
              }
            break;
            case 12:
              if (select == true) {
                select = false;
                msgToSend[index] = 'k';
                index++;
              }
            break;
            case 24:
              if (select == true) {
                select = false;
                msgToSend[index] = 'l';
                index++;
              }
            break;
            case 36:
              if (select == true) {
                select = false;
                msgToSend[index] = 'm';
                index++;
              }
            break;
            case 48:
              if (select == true) {
                select = false;
                msgToSend[index] = 'n';
                index++;
              }
            break;
            case 60:
              if (select == true) {
                select = false;
                msgToSend[index] = 'o';
                index++;
              }
            break;
            case 72:
              if (select == true) {
                select = false;
                msgToSend[index] = 'p';
                index++;
              }
            break;
            case 84:
              if (select == true) {
                select = false;
                msgToSend[index] = 'q';
                index++;
              }
            break;
            case 96:
              if (select == true) {
                select = false;
                msgToSend[index] = 'r';
                index++;
              }
            break;
            case 108:
              if (select == true) {
                select = false;
                capitalize = true;
                
              }
            break;
          }
        break;
  ///////////////////////////////////////
        case 53:
          switch (x) {
            case 0:
              if (select == true) {
                select = false;
                msgToSend[index] = 's';
                index++;
              }
            break;
            case 12:
              if (select == true) {
                select = false;
                msgToSend[index] = 't';
                index++;
              }
            break;
            case 24:
              if (select == true) {
                select = false;
                msgToSend[index] = 'u';
                index++;
              }
            break;
            case 36:
              if (select == true) {
                select = false;
                msgToSend[index] = 'v';
                index++;
              }
            break;
            case 48:
              if (select == true) {
                select = false;
                msgToSend[index] = 'w';
                index++;
              }
            break;
            case 60:
              if (select == true) {
                select = false;
                msgToSend[index] = 'x';
                index++;
              }
            break;
            case 72:
              if (select == true) {
                select = false;
                msgToSend[index] = 'y';
                index++;
              }
            break;
            case 84:
              if (select == true) {
                select = false;
                msgToSend[index] = 'z';
                index++;
              }
            break;
            case 96:
              if (select == true) {
                select = false;
                msgToSend[index] = ' ';
                index++;
              }
            break;
            case 108:
              if (select == true) {
                select = false;
                index--;
                msgToSend[index] = ' ';
              }
            break;
          }//switch x
        break;
      }//switch y
    }//if
  //////////////////////////////////////////////////////////////////////////////////////////////
    else if (capitalize == true && letters == true) {
      switch (y) {
        case 23:
          if (select == true) {
            select = false;
            transmit = true;
            num++;
            
          }
        case 33:
          switch (x) {
            case 0:
              if (select == true) {
                select = false;
                msgToSend[index] = 'A';
                index++;
              }
            break;
            case 12:
              if (select == true) {
                select = false;
                msgToSend[index] = 'B';
                index++;
              }
            break;
            case 24:
              if (select == true) {
                select = false;
                msgToSend[index] = 'C';
                index++;
              }
            break;
            case 36:
              if (select == true) {
                select = false;
                msgToSend[index] = 'D';
                index++;
              }
            break;
            case 48:
              if (select == true) {
                select = false;
                msgToSend[index] = 'E';
                index++;
              }
            break;
            case 60:
              if (select == true) {
                select = false;
                msgToSend[index] = 'F';
                index++;
              }
            break;
            case 72:
              if (select == true) {
                select = false;
                msgToSend[index] = 'G';
                index++;
              }
            break;
            case 84:
              if (select == true) {
                select = false;
                msgToSend[index] = 'H';
                index++;
              }
            break;
            case 96:
              if (select == true) {
                select = false;
                msgToSend[index] = 'I';
                index++;
              }
            break;
            case 108:
              if (select == true) {
                select = false;
                letters = false;
                
              }
            break;
          }
        break;
  ///////////////////////////////////////
        case 43:
          switch (x) {
            case 0:
              if (select == true) {
                select = false;
                msgToSend[index] = 'J';
                index++;
              }
            break;
            case 12:
              if (select == true) {
                select = false;
                msgToSend[index] = 'K';
                index++;
              }
            break;
            case 24:
              if (select == true) {
                select = false;
                msgToSend[index] = 'L';
                index++;
              }
            break;
            case 36:
              if (select == true) {
                select = false;
                msgToSend[index] = 'M';
                index++;
              }
            break;
            case 48:
              if (select == true) {
                select = false;
                msgToSend[index] = 'N';
                index++;
              }
            break;
            case 60:
              if (select == true) {
                select = false;
                msgToSend[index] = 'O';
                index++;
              }
            break;
            case 72:
              if (select == true) {
                select = false;
                msgToSend[index] = 'P';
                index++;
              }
            break;
            case 84:
              if (select == true) {
                select = false;
                msgToSend[index] = 'Q';
                index++;
              }
            break;
            case 96:
              if (select == true) {
                select = false;
                msgToSend[index] = 'R';
                index++;
              }
            break;
            case 108:
              if (select == true) {
                select = false;
                capitalize = false;
                
              }
            break;
          }
        break;
  ///////////////////////////////////////
        case 53:
          switch (x) {
            case 0:
              if (select == true) {
                select = false;
                msgToSend[index] = 'S';
                index++;
              }
            break;
            case 12:
              if (select == true) {
                select = false;
                msgToSend[index] = 'T';
                index++;
              }
            break;
            case 24:
              if (select == true) {
                select = false;
                msgToSend[index] = 'U';
                index++;
              }
            break;
            case 36:
              if (select == true) {
                select = false;
                msgToSend[index] = 'V';
                index++;
              }
            break;
            case 48:
              if (select == true) {
                select = false;
                msgToSend[index] = 'W';
                index++;
              }
            break;
            case 60:
              if (select == true) {
                select = false;
                msgToSend[index] = 'X';
                index++;
              }
            break;
            case 72:
              if (select == true) {
                select = false;
                msgToSend[index] = 'Y';
                index++;
              }
            break;
            case 84:
              if (select == true) {
                select = false;
                msgToSend[index] = 'Z';
                index++;
              }
            break;
            case 96:
              if (select == true) {
                select = false;
                msgToSend[index] = ' ';
                index++;
              }
            break;
            case 108:
              if (select == true) {
                select = false;
                index--;
                msgToSend[index] = ' ';
              }
            break;
          }//switch x
        break;
      }//switch y
    }//else if
  /////////////////////////////////////////////////////////////////////////////////////////////
    else if (letters == false) {
      switch (y) {
        case 23:
          if (select == true) {
            select = false;
            transmit = true;
            num++;
          }
        case 33:
          switch (x) {
            case 0:
              if (select == true) {
                select = false;
                msgToSend[index] = '1';
                index++;
              }
            break;
            case 12:
              if (select == true) {
                select = false;
                msgToSend[index] = '2';
                index++;
              }
            break;
            case 24:
              if (select == true) {
                select = false;
                msgToSend[index] = '3';
                index++;
              }
            break;
            case 36:
              if (select == true) {
                select = false;
                msgToSend[index] = '4';
                index++;
              }
            break;
            case 48:
              if (select == true) {
                select = false;
                msgToSend[index] = '5';
                index++;
              }
            break;
            case 60:
              if (select == true) {
                select = false;
                msgToSend[index] = '6';
                index++;
              }
            break;
            case 72:
              if (select == true) {
                select = false;
                msgToSend[index] = '7';
                index++;
              }
            break;
            case 84:
              if (select == true) {
                select = false;
                msgToSend[index] = '8';
                index++;
              }
            break;
            case 96:
              if (select == true) {
                select = false;
                msgToSend[index] = '9';
                index++;
              }
            break;
            case 108:
              if (select == true) {
                select = false;
                letters = true;
              }
            break;
          }
        break;
  ///////////////////////////////////////
        case 43:
          switch (x) {
            case 0:
              if (select == true) {
                select = false;
                msgToSend[index] = '0';
                index++;
              }
            break;
            case 12:
              if (select == true) {
                select = false;
                msgToSend[index] = '#';
                index++;
              }
            break;
            case 24:
              if (select == true) {
                select = false;
                msgToSend[index] = '&';
                index++;
              }
            break;
            case 36:
              if (select == true) {
                select = false;
                msgToSend[index] = '%';
                index++;
              }
            break;
            case 48:
              if (select == true) {
                select = false;
                msgToSend[index] = '+';
                index++;
              }
            break;
            case 60:
              if (select == true) {
                select = false;
                msgToSend[index] = '-';
                index++;
              }
            break;
            case 72:
              if (select == true) {
                select = false;
                msgToSend[index] = '=';
                index++;
              }
            break;
            case 84:
              if (select == true) {
                select = false;
                msgToSend[index] = '/';
                index++;
              }
            break;
            case 96:
              if (select == true) {
                select = false;
                msgToSend[index] = '*';
                index++;
              }
            break;
            case 108:
              if (select == true) {
                select = false;
                capitalize = true;
              }
            break;
          }
        break;
  ///////////////////////////////////////
        case 53:
          switch (x) {
            case 0:
              if (select == true) {
                select = false;
                msgToSend[index] = '.';
                index++;
              }
            break;
            case 12:
              if (select == true) {
                select = false;
                msgToSend[index] = ',';
                index++;
              }
            break;
            case 24:
              if (select == true) {
                select = false;
                msgToSend[index] = '?';
                index++;
              }
            break;
            case 36:
              if (select == true) {
                select = false;
                msgToSend[index] = '!';
                index++;
              }
            break;
            case 48:
              if (select == true) {
                select = false;
                msgToSend[index] = '\'';
                index++;
              }
            break;
            case 60:
              if (select == true) {
                select = false;
                msgToSend[index] = ':';
                index++;
              }
            break;
            case 72:
              if (select == true) {
                select = false;
                msgToSend[index] = ';';
                index++;
              }
            break;
            case 84:
              if (select == true) {
                select = false;
                msgToSend[index] = '(';
                index++;
              }
            break;
            case 96:
              if (select == true) {
                select = false;
                msgToSend[index] = ')';
                index++;
              }
            break;
            case 108:
              if (select == true) {
                select = false;
                index--;
                msgToSend[index] = ' ';
              }
            break;
          }//switch x
        break;
      }//switch y
    }//else if
    if (index <= 0) {
      index = 0;
    }
    if (index > 29) {
      index = 29;
    }
    display.setCursor(0, 0);
    display.println(msgToSend);
    display.setCursor(0, 23);
    display.println(index);
  }
}//drawText

void WritterV2::drawBlink(void) {
  if (show == true) {
    if (xBlink <= 125) {
      xBlink = index * 6;
      yBlink1 = 0;
      yBlink2 = 6;
    }
    if (xBlink >= 125) {
      xBlink = (index - 21) * 6;
      yBlink1 = 8;
      yBlink2 = 14;
    }
    if (blinkDelay <=10) {
      display.drawLine(xBlink, yBlink1, xBlink, yBlink2, SH110X_WHITE);//(x1, y1, x2, y2) verticle
    }
    if (blinkDelay >= 20) {
      blinkDelay = 0;
    }
  }
}//drawBlink

const char* WritterV2::stringFinished(void){
  while( booleanStringFinished == false ){
    display.clearDisplay();
    timeDelay();
    drawMenu();
    drawCursor();
    drawText();
    drawBlink();
    display.display();
  }
  booleanStringFinished = false; // Reset de FLAG string terminado para luego permitir nuevamente su uso
  return msgToSend[0] != '\0' ? msgToSend : nullptr;
  
}

Writter::Writter(Adafruit_SH1106G* object) : display(*object){}

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
  display.println(F("OK"));
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
  
  while(INFINITE_LOOPING){

    Key_pressed=true;
    if ( buttonState(PIN::Buttons::BACK)  == HIGH )               {To_Transmit = ""; break;}                //Boton BACK
    if ((buttonState(PIN::Buttons::LEFT)  == HIGH ) and Old_X>0)  { New_X=Old_X-1; delay(DEBOUNCE_TIME);}   //Boton LEFT
    if ((buttonState(PIN::Buttons::RIGHT) == HIGH ) and Old_X<9)  { New_X=Old_X+1; delay(DEBOUNCE_TIME);}   //Boton RIGHT (Aca es donde se encuentra el bug del boton derecho)
    if ((buttonState(PIN::Buttons::UP)    == HIGH ) and Old_Y>-1) { New_Y=Old_Y-1; delay(DEBOUNCE_TIME);}   //Boton UP
    if ((buttonState(PIN::Buttons::DOWN)  == HIGH ) and  Old_Y<2 ){ New_Y=Old_Y+1; delay(DEBOUNCE_TIME);}   //Boton DOWN
    if (buttonState(PIN::Buttons::RIGHT)  == HIGH ) {                              delay(DEBOUNCE_TIME);    //Boton RIGHT (Aca es donde se encuentra el bug del boton derecho)
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
        delay(DEBOUNCE_TIME);
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
      Old_X=New_X;
      Old_Y=New_Y;;
      
    }
    if ((Old_X!=New_X or Old_Y!=New_Y) and Old_Y!=-1 ){
      if (New_Y!=-1 )Highlight_letter (New_X,Old_X,New_Y,Old_Y);
      Old_X=New_X;
      Old_Y=New_Y;
    }

    display.display();
  }

}

const char* Writter::stringFinished(void){ 
  Writter::Graphics();
  Writter::keyboard();
  if(To_Transmit.c_str() == "")
    return nullptr;
  return To_Transmit.c_str(); 
}
