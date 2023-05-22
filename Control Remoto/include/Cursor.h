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

#include <Arduino.h>
#include <stdint.h>
#include <Adafruit_SH110X.h>
#include "Interface.h"

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
  #define buttonState(PIN_BUTTON) digitalRead(PIN_BUTTON) 
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


/*! \deprecated Cursor nuevo desarrollado llamado: CursorV2
    @brief Cursor que se desplaza a traves del display
    @note Posee de dos constructores
    @note 1- Para desplazarse entre los menus del display
    @note 2- Para la creacion de strings que recibe del usuario
*/
class [[deprecated("Se reemplazo por CursorV2, esta en desuso esta clase")]] Cursor {

private:
  const uint8_t LINE_CURSOR_X = 10;
  const char CURSOR_CHARACTER = '>';
  const char CURSOR_CHARACTER_DELETE = 'X';
  uint8_t AMOUNT_OF_OPTIONS;
  Adafruit_SH1106G &display;

public:
  /*! @brief Constructor para uso de desplazamiento de opciones
      @param  AMOUNT_OF_OPTIONS__
      Cantidad de opciones que puede desplazarse el cursor  */
  Cursor(const uint8_t AMOUNT_OF_OPTIONS__ , Adafruit_SH1106G &displayObject);
  /*! @brief Constructor para uso de almacenamiento de strings del usuario  */
  //Cursor(void);
  /*! @brief Cursor que responde en coorcondansia con la botonera
      @return El numero de la opcion seleccionada (en caso de presionar el boton BACK, retornara 0) */
  const uint8_t options(void);

  const char* writer_ptr(void);

};

/*! @brief Cursor que se desplaza a traves del display (desarrollado v2)*/
class CursorV2 {
  private:
    char** options;
    Adafruit_SH1106G* sh1106;
    int currentIndex;
    int totalPages;
    int currentPage;

  public:
    /*! @brief Constructor
        @param menuOptions Doble puntero que apunta a array de punteros que apuntan a strings (las opciones)
        @param display Recibe referencia o puntero al objeto del display
    */
    CursorV2(char** menuOptions, Adafruit_SH1106G* display);

    /*! @brief Obtiene el numero total strings
        @returns Numero total de punteros que hay dentro del array
    */
    int getNumberOfOptions();

    /*! @brief Pagina/apartado en el que se encuentra el cursor
        @note Lo muestra en el screen Oled
    */
    void showCurrentPage();

    /*! @brief Inicia bucle infinito 
        @returns
        @note Lo muestra en el screen Oled
    */
    char* getSelectedOption();
};

/*! @brief Cursor que se desplaza a traves del display (desarrollado v2)*/
class [[deprecated("Se reemplazo por WritterV2, esta en desuso esta clase")]] Writter{

  private:
  //Referencia al objeto del display
    Adafruit_SH1106G& display;
  //Potentiometer PIN A1
    int Keyboard=A7;
  // Variables capturing current and newly calculated position on the letter board (9x3 - 27 postions)
    int New_X=0;
    int Old_X=0;
    int New_Y=0;
    int Old_Y=0;
  // Variable capturing output from Keyboard pin (Values 0 1023)
    int Key_read=0;
    int Prev_Key_read=1023;
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

  Writter(Adafruit_SH1106G& display);

  String stringFinished(void);

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
  const int leftPin = 2;
  const int rightPin = 3;
  const int upPin = 4;
  const int downPin = 5;
  const int selectPin = 6;
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

  //drawBlink
  int xBlink = 0;
  int yBlink1 = 0;
  int yBlink2 = 0;

  public:

  WritterV2(Adafruit_SH1106G* displayReference);

  inline void setup(void);
  inline void loop(void);
  void atcSetup(void);
  void atcLoop(void);
  void timeDelay(void);
  void drawMenu(void);
  void drawCursor(void);
  void drawText(void);
  void drawBlink(void);
  String stringFinished(void);

};

#endif //Cursor_h