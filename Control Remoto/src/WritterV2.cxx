//Copyright Grupo 7, Inc. All Rights Reserved.
/***********************************************
 * * * * * * * * * * * * * * * * * * * * * * * *
 * \file
 * Writter (escritor de strings)
 * usa los botones como perisfericos
 * Permiten: Mayusculas, Minusculas y signos (NO INCLUYE EL ESPACIO!!! -> ' ')
 * 
 * * * * * * * * * * * * * * * * * * * * * * * *
***********************************************/

#include "Cursor.h"

WritterV2::WritterV2(Adafruit_SH1106G* displayReference) : display_ptr(displayReference) {  }

///////////////////////
//      -SETUP-      //
///////////////////////
inline void WritterV2::setup(void) {
  atcSetup();
}//setup

//////////////////////
//      -LOOP-      //
//////////////////////
inline void WritterV2::loop(void) {
  atcLoop();
}//loop

///////////////////////////
//      -FUNCTIONS-      //
///////////////////////////
void WritterV2::atcSetup(void) {
  //OLED Setup
  //display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.begin(0x3C, true);
  display.display();
  delay(500);
  display.clearDisplay();
  //Pin Setup
  pinMode(leftPin, INPUT_PULLUP);
  pinMode(rightPin, INPUT_PULLUP);
  pinMode(upPin, INPUT_PULLUP);
  pinMode(downPin, INPUT_PULLUP);
  pinMode(selectPin, INPUT_PULLUP);
  pinMode(switchPin, INPUT_PULLUP);
  //Begin Serial Communication
  //Serial.begin(9600);
}//atcSetup

void WritterV2::atcLoop(void) {
  display.clearDisplay();
  timeDelay();
  drawMenu();
  drawCursor();
  drawText();
  drawBlink();
  display.display();
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
    display.println("snd");
    display.setCursor(109, 35);
    display.println("ext");
    display.setCursor(109, 45);
    display.println("cap");
    display.setCursor(109, 55);
    display.println("del");
    if (letters == true && capitalize == false) {
      display.setCursor(1, 35);
      display.println("a b c d e f g h i");
      display.setCursor(1, 45);
      display.println("j k l m n o p q r");
      display.setCursor(1, 55);
      display.println("s t u v w x y z _");
    }
    if (letters == true && capitalize == true) {
      display.setCursor(1, 35);
      display.println("A B C D E F G H I");
      display.setCursor(1, 45);
      display.println("J K L M N O P Q R");
      display.setCursor(1, 55);
      display.println("S T U V W X Y Z _");
    }
    if (letters == false) {
      display.setCursor(1, 35);
      display.println("1 2 3 4 5 6 7 8 9");
      display.setCursor(1, 45);
      display.println("0 # & % + - = / *");
      display.setCursor(1, 55);
      display.println(". , ? ! ' : ; ( )");
    }
  }
}//drawMenu

void WritterV2::drawCursor(void) {
  if (buttonDelay >= 3 && show == true) {
    if(digitalRead(upPin) == LOW) {
      y -= 10;
      buttonDelay = 0;
      transmit = false;
    }
    if(digitalRead(downPin) == LOW) {
      y += 10;
      buttonDelay = 0;
      transmit = false;
    }
    if(digitalRead(leftPin) == LOW) {
      x -= 12;
      buttonDelay = 0;
      transmit = false;
    }
    if(digitalRead(rightPin) == LOW) {
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
  if (selectDelay >= 3 && digitalRead(selectPin) == LOW) {
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

String WritterV2::stringFinished(void){

  while(true) atcLoop();

  return msgToSend;

}