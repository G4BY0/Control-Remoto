//Copyright Grupo 11, Inc. All Rights Reserved.
/***********************************************
 * * * * * * * * * * * * * * * * * * * * * * * *
 * 
 * A class that permits you to show a short\long texts separated in pages.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * *
***********************************************/
#ifndef BOOK_H
#define BOOK_H

#include <Arduino.h>
#include <Adafruit_SH110X.h>
#include <pgmspace.h> // Biblioteca para acceder a datos almacenados en PROGMEM 
#include "PIN.h"
#include "Interface.h"

/*! \brief Libreta que muestra texto*/
class Book {
public:
  /*! \brief Constructor
      \param bookContent Contenido del libro
      \param display Objeto para el manejo de pantalla*/
  explicit Book(const __FlashStringHelper * bookConten_P, Adafruit_SH1106G* display);

  /*! \brief Constructor
      \param bookContent Contenido del libro
      \param display Objeto para el manejo de pantalla*/
  explicit Book(const char* bookContent, Adafruit_SH1106G* display);

  void update();

private:
  Adafruit_SH1106G* __display;
  const uint8_t PIN_NEXT = PIN::Buttons::RIGHT;
  const uint8_t PIN_PREV = PIN::Buttons::LEFT;
  uint16_t currentPage = 1;
  uint16_t totalPages;
  const char* bookContent;
  
private:
  const uint16_t calculate_total_pages_P(void);
  const uint16_t calculate_total_pages(void);
  void nextPage(void);
  void prevPage(void);
  void displayPage(void);

};


#endif // BOOK_H