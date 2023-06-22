//Copyright Grupo 7, Inc. All Rights Reserved.
/***********************************************
 * * * * * * * * * * * * * * * * * * * * * * * *
 * \file
 * Cursor de desplazamiento de los menus 
 * 
 * * * * * * * * * * * * * * * * * * * * * * * *
***********************************************/

#ifndef CURSOR_H
#define CURSOR_H

#include <Arduino.h> //Arduino Framework
#include <stdint.h> //Tipo de Enteros
#include <inttypes.h> //Tipo de Enteros
#include <Adafruit_SH110X.h> // Lib de display Oled
#include "Interface.h" // Interfaz del usuario

//Cursor
#define MAX_LINE_OPTIONS_OUTPUT 3
#define SPACE_FOR_PUSSYS '_'
#define LINE_STRING_X 20
const uint8_t LINE_STRING_Y[MAX_LINE_OPTIONS_OUTPUT] = {10,20,50};


//CursorV2 Macros
#define UP_BUTTON_PIN PIN::Buttons::UP
#define DOWN_BUTTON_PIN PIN::Buttons::DOWN
#define ENTER_BUTTON_PIN PIN::Buttons::ENTER
#define BACK_BUTTON_PIN PIN::Buttons::BACK


//Writter Macros
#ifndef buttonState_function_Macro
  #define buttonState_function_Macro
  /*! @brief Estado logico del pin de la placa de desarrollo
    @param PIN_BUTTON 
           Pin de la placa de desarrollo
    @returns Estado logico del pin de la placa de desarrollo */
  #define buttonState(PIN_BUTTON) !digitalRead(PIN_BUTTON) 
#endif
#define UP_BUTTON_PIN PIN::Buttons::UP
#define DOWN_BUTTON_PIN PIN::Buttons::DOWN
#define LEFT_BUTTON_PIN PIN::Buttons::LEFT
#define RIGHT_BUTTON_PIN PIN::Buttons::RIGHT
#define ENTER_BUTTON_PIN PIN::Buttons::ENTER
#define BACK_BUTTON_PIN PIN::Buttons::BACK
// Oled display size
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Mario's Ideas
// Text input using OLED display an 5 Key Keyboard

namespace CursorUltimate{

  const char* getOption( std::vector<std::string> Strings , const size_t CantidadDeStrings , Adafruit_SH1106G* OLEDObject_ptr );

}

/*! @brief Cursor que se desplaza a traves del display (desarrollado v2)*/
class CursorV2 {
  private:
    std::vector<std::string> options;
    Adafruit_SH1106G* sh1106;
    uint currentIndex;
    uint totalPages;
    uint currentPage;

  public:
    /*! @brief Constructor
        @param menuOptions objeto de caracter std::vector<std::string> que contiene las opciones de trabajo para el cursor
        @param display Recibe referencia o puntero al objeto del display
    */
    CursorV2(std::vector<std::string> menuOptions, Adafruit_SH1106G* display);

    /*! @brief Obtiene el numero total strings
        @returns Numero total de punteros que hay dentro del array
    */
    const size_t getNumberOfOptions(void) const;

    /*! @brief Pagina/apartado en el que se encuentra el cursor
        @note Lo muestra en el screen Oled
    */
    void showCurrentPage(void);

    /*! @brief Inicia bucle infinito 
        @returns
        @note Lo muestra en el screen Oled
    */
    const char* getSelectedOption(void);
};

/*! @brief Cursor que se desplaza a traves del display (desarrollado v2)*/
class [[deprecated("Se reemplazo por WritterV2, esta en desuso y tiene errores")]] Writter{

  private:
  //Referencia al objeto del display
    Adafruit_SH1106G& display;
  //Potentiometer PIN A1

  // Variables capturing current and newly calculated position on the letter board (9x3 - 27 postions)
    int New_X=0;
    int Old_X=0;
    int New_Y=0;
    int Old_Y=0;
  // Variable capturing output from Keyboard pin (Values 0 1023)


    boolean Key_pressed=false; 
  // String variable holding the text to transmit
    String To_Transmit="";
  // Length of the text to transmit
    int To_Transmit_Length=0;


  // Used for displaying Leter board
  char Letters[3][10]={"ABCDEFGHI",
                       "JKLMNOPQR",
                       "STUVWXYZ_" };
  
  void Graphics(void);
  void Highlight_letter(int X, int X_Old, int Y, int Y_Old);
  void keyboard(void);


  public:

  Writter(Adafruit_SH1106G* display);

  const char* stringFinished(void);

};

class WritterV2{

  private:
  Adafruit_SH1106G* display_ptr;
  Adafruit_SH1106G& display = *display_ptr;
  ///////////////////////////
  //      -VARIABLES-      //
  ///////////////////////////
  //Data to Send
  struct dataPackage {
    char msg[30];
    int msgLen[1];
  };
  dataPackage data;

  //Pins
  const int leftPin = PIN::Buttons::LEFT;
  const int rightPin = PIN::Buttons::RIGHT;
  const int upPin = PIN::Buttons::UP;
  const int downPin = PIN::Buttons::DOWN;
  const int selectPin = PIN::Buttons::ENTER;
  const int switchPin = 7;

  //timeDelay
  int buttonDelay = 0;
  int selectDelay = 0;
  int radioDelay = 0;
  int blinkDelay = 0;

  //drawMenu
  bool show = true;
  bool letters = true;
  bool capitalize = false;

  //drawCursor
  int x = 0;
  int y = 33;
  int width = 7;
  int height = 11;
  bool select = false;
  bool transmit = false;

  //drawText
  int index = 0;
  char msgToSend[30];
  int num = 0;

  //receiveText
  int msgFlag = 0;
  char convertedMsg[30];
  bool displayMsg = false;
  bool ringFlag = true;
  bool booleanStringFinished = false;

  //drawBlink
  int xBlink = 0;
  int yBlink1 = 0;
  int yBlink2 = 0;

  public:

  WritterV2(Adafruit_SH1106G* displayReference);

  inline void loop(void);
  void atcLoop(void);
  void timeDelay(void);
  void drawMenu(void);
  void drawCursor(void);
  void drawText(void);
  void drawBlink(void);
  const char* stringFinished(void);

};

#endif //Cursor_h