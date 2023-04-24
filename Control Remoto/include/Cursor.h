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

/*! @brief Cursor que se desplaza a traves del display
    @note Posee de dos constructores
    @note 1- Para desplazarse entre los menus del display
    @note 2- Para la creacion de strings que recibe del usuario*/
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


#endif //Cursor_h