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

#include "Interface.h"
#include <Adafruit_SH110X.h>

/*! @brief Cursor que se desplaza a traves del display
    @note Posee de dos constructores
    @note 1- Para desplazarse entre los menus del display
    @note 2- Para la creacion de strings que recibe del usuario
    \deprecated Cursor nuevo desarrollado llamado: CursorV2*/
class Cursor {

private:
  const uint8_t LINE_CURSOR_X = 10;
  const char CURSOR_CHARACTER = '>';
  const char CURSOR_CHARACTER_DELETE = 'X';
  
  uint8_t AMOUNT_OF_OPTIONS;

public:
  /*! @brief Constructor para uso de desplazamiento de opciones
      @param  AMOUNT_OF_OPTIONS__
      Cantidad de opciones que puede desplazarse el cursor  */
  Cursor(const uint8_t AMOUNT_OF_OPTIONS__);
  /*! @brief Constructor para uso de almacenamiento de strings del usuario  */
  Cursor(void);
  /*! @brief Cursor que responde en coorcondansia con la botonera
      @return El numero de la opcion seleccionada (en caso de presionar el boton BACK, retornara 0) */
  const uint8_t options(void);

  const char* writer_ptr(void);

};

/*! @brief Cursor que se desplaza a traves del display (desarrollado v2)*/

#define UP_BUTTON_PIN PIN::Buttons::UP
#define DOWN_BUTTON_PIN PIN::Buttons::DOWN
#define ENTER_BUTTON_PIN PIN::Buttons::ENTER
#define BACK_BUTTON_PIN PIN::Buttons::BACK

class CursorV2 {
  private:
    char** options;
    Adafruit_SH1106G* sh1106;
    int currentIndex;
    int totalPages;
    int currentPage;
  public:
    CursorV2(char** menuOptions, Adafruit_SH1106G* display);

    int getNumberOfOptions();

    void showCurrentPage();

    char* getSelectedOption();
};


#endif //Cursor_h