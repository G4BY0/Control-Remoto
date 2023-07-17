//Copyright Grupo 11, Inc. All Rights Reserved.
/***********************************************
 * * * * * * * * * * * * * * * * * * * * * * * *
 * 
 * A class that permits you to show a short\long texts separated in pages.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * *
***********************************************/
#ifndef BOOK_H

#include <Adafruit_SSD1306.h>
#include <pgmspace.h> // Biblioteca para acceder a datos almacenados en PROGMEM 

#include "Interface.h"
#include "Cursor.h"
#include "Tasks.h"



#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
/*! \brief Libreta que muestra texto*/
class Book {
public:
  /*! \brief Constructor
      \param bookContent Contenido del libro
      \param display Objeto para el manejo de pantalla*/
  Book(const char* bookContent, Adafruit_SH1106G* display) : bookContent(bookContent), display(display) {
    totalPages = calculateTotalPages(); //Calcula el total de paginas que tendra el Libro
    currentPage = 1; //Inicializa en la primer pagina
  }

  void update() {
    if (buttonState(PIN::Buttons::RIGHT) == true || buttonState(PIN::Buttons::DOWN) == true) {
      nextPage();
      delay(DEBOUNCE_TIME);
    }

    if (buttonState(PIN::Buttons::LEFT) == true || buttonState(PIN::Buttons::UP) == true) {
      prevPage();
      delay(DEBOUNCE_TIME);
    }
  }

private:
  Adafruit_SH1106G* display;
  const int PIN_NEXT = 2;
  const int PIN_PREV = 3;
  int currentPage;
  int totalPages;
  const char* bookContent;

  int calculateTotalPages() {
    int charsPerPage = SCREEN_WIDTH * SCREEN_HEIGHT / (6 * 8);
    int contentLength = strlen_P(bookContent);
    int totalPages = contentLength / charsPerPage;
    if (contentLength % charsPerPage != 0) {
      totalPages++;
    }
    return totalPages;
  }

  void nextPage() {
    if (currentPage < totalPages) {
      currentPage++;
      displayPage();
    }
  }

  void prevPage() {
    if (currentPage > 1) {
      currentPage--;
      displayPage();
    }
  }

  void displayPage() {
    display->clearDisplay();
    display->setCursor(0, 0);
    unsigned int charsPerPage = SCREEN_WIDTH * SCREEN_HEIGHT / (6 * 8);
    unsigned int startIndex = (currentPage - 1) * charsPerPage;
    unsigned int endIndex = startIndex + charsPerPage;

    if (endIndex > strlen(bookContent)) 
    endIndex = strlen(bookContent);
    
    for (int i = startIndex; i < endIndex; i++) 
    display->write(bookContent[i]);
    
    display->display();
  }

};


#endif // BOOK_H