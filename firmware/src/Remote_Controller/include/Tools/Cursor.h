//Copyright Grupo 11, Inc. All Rights Reserved.
/***********************************************
 * * * * * * * * * * * * * * * * * * * * * * * *
 * \file
 *  ### Cursor.h ### (de desplazamiento de los menus) & Writter (Teclado Virtual Para la escritura de Strings)
 *  - Cursor         ( (BUGS) En Uso, Con un bug de que no se puede ir desde la ultima posicion hacia la primera, Pulsando el boton DOWN)
 *  - CursorUltimate ( (BUGS) En desarrollo y desuso, se penso su uso por problemas que tenia el CursorV2 pero fueron resueltos)
 *  - WritterV2      ( (BUGS) En Uso, bastante estetico pero aun tiene algunos bugs que son como el boton snd que solo funciona cuando el teclado esta en minuscula)
 * * * * * * * * * * * * * * * * * * * * * * * *
************************************************/
#ifndef CURSOR_H
#define CURSOR_H

#include <Arduino.h>            //Arduino Framework
#include <cstdint>              //Tipo de Enteros
#include <vector>
#include <Adafruit_SH110X.h>    //Lib de display Oled
#include "Interface.h"
#include "System.h"              //Para el manejo del mutex del display

// Mario's Ideas
// Text input using OLED display an 5 Key Keyboard

/*! @brief Cursor que se desplaza a traves del display*/
class Cursor {
public:

  /*! \brief Constructor
      \param menuOptions objeto de caracter std::vector<std::string> que contiene las opciones de trabajo para el cursor
      \param display Recibe referencia o puntero al objeto del display
      \param inicialOption Opcion Inicial del Cursor
      \param MAX_LINE_OPTIONS_OUTPUT Maximo de lineas de opciones que se deberian mostrar por pagina
      \param _x Es la coordenada de abscisas de origen en la que las opciones se pueden mostrar hacia la derecha
      \param _y Es la coordenada de ordenada de origen en la que las opciones se pueden mostrar hacia abajo
      \param inicialOption Opcion Inicial del Cursor
  */
  Cursor(const std::vector<std::string>& menuOptions ,Adafruit_SH1106G& display ,const uint8_t inicialOption = 0U ,const uint8_t MAX_LINE_OPTIONS_OUTPUT = 5U ,const uint16_t _x = 0U ,const uint16_t _y = 10U);

  /*! \brief Constructor, si la opcion inicial = 0
      \param menuOptions objeto de caracter std::vector<std::string> que contiene las opciones de trabajo para el cursor
      \param display Recibe referencia o puntero al objeto del display
      \param MAX_LINE_OPTIONS_OUTPUT Maximo de lineas de opciones que se deberian mostrar por pagina
      \param _x Es la coordenada de abscisas de origen en la que las opciones se pueden mostrar hacia la derecha
      \param _y Es la coordenada de ordenada de origen en la que las opciones se pueden mostrar hacia abajo
      \param inicialOption Opcion Inicial del Cursor
  */
  Cursor(const std::vector<String>& menuOptions ,Adafruit_SH1106G& display ,const uint8_t MAX_LINE_OPTIONS_OUTPUT = 5U ,const uint16_t _x = 0U ,const uint16_t _y = 10U);

  /*! \brief Inicia bucle infinito 
      \returns
      \note Lo muestra en el screen Oled
  */
  const char* getSelectedOption(void);

private:
  
  std::vector<std::string> options;
  Adafruit_SH1106G& __display;  // display reference to the object
  unsigned currentIndex = 0U;        // posicion de las opciones
  unsigned totalPages;              // total de paginas
  unsigned currentPage = 0U;         // posicion de pagina
  const uint _x ,_y;
  const uint8_t MAX_LINE_OPTIONS_OUTPUT;

private:
  /*! \brief Obtiene el numero total strings
      \returns Numero total de opciones
  */
  size_t getNumberOfOptions(void) const;
  
  /*! \brief Pagina/apartado en el que se encuentra el cursor
      \note Lo muestra en el screen Oled
  */
  void showCurrentPage(void);

  /*! \brief Clerea la pagina
  */
  void clearPage(void);
};



/*! \brief Cursor que Se planteaba usar y quedo en desuso
    \deprecated Reemplazado por "Cursor" */
namespace CursorUltimate{

  /*! \brief Proceso de seleccion para el usuario
      \return String seleccionado
      \note Si se cancela la seleccion, retorna 'nullptr'
      \deprecated Reemplazado por "Cursor" */
  const char* [[deprecated("Se reemplazo por Cursor, esta en desuso, no esta terminado y tiene errores")]] getOption( std::vector<std::string> Strings , const size_t CantidadDeStrings , Adafruit_SH1106G* OLEDObject_ptr );

}

#endif //Cursor_h