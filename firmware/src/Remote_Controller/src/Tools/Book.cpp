//Copyright Grupo 11, Inc. All Rights Reserved.
/***********************************************
 * * * * * * * * * * * * * * * * * * * * * * * *
 * 
 * Source of Book class
 * 
 * * * * * * * * * * * * * * * * * * * * * * * *
***********************************************/

#include "Tools/Book.h"

Book::Book(const __FlashStringHelper * bookConten_P, Adafruit_SH1106G* display) : bookContent(reinterpret_cast<const char*>(bookConten_P)), __display(display) {
  totalPages = calculate_total_pages_P(); //Calcula el total de paginas que tendra el Libro
  currentPage = 1U; //Inicializa en la primer pagina
}

/*! \brief Constructor
    \param bookContent Contenido del libro
    \param display Objeto para el manejo de pantalla*/
Book::Book(const char* bookContent, Adafruit_SH1106G* display) : bookContent(bookContent), __display(display) {
  totalPages = calculate_total_pages_P(); //Calcula el total de paginas que tendra el Libro
  currentPage = 1U; //Inicializa en la primer pagina
}


void Book::update() {
  if (buttonState(PIN::Buttons::RIGHT) == true || buttonState(PIN::Buttons::DOWN) == true) {
    nextPage();
    delay(DEBOUNCE_TIME);
  }

  if (buttonState(PIN::Buttons::LEFT) == true || buttonState(PIN::Buttons::UP) == true) {
    prevPage();
    delay(DEBOUNCE_TIME);
  }
}

const uint16_t Book::calculate_total_pages_P(void) {
  int charsPerPage = SCREEN_WIDTH * SCREEN_HEIGHT / (6 * 8);
  int contentLength = strlen_P(bookContent);
  int totalPages = contentLength / charsPerPage;
  if (contentLength % charsPerPage != 0) {
    totalPages++;
  }
  return totalPages;
}

const uint16_t Book::calculate_total_pages(void) {
  uint16_t charsPerPage = SCREEN_WIDTH * SCREEN_HEIGHT / (6U * 8U);
  uint16_t contentLength = strlen(bookContent);
  int totalPages = contentLength / charsPerPage;
  if (contentLength % charsPerPage != 0) {
    totalPages++;
  }
  return totalPages;
}

void Book::nextPage(void) {
  if (currentPage < totalPages) {
    currentPage++;
    displayPage();
  }
}

void Book::prevPage(void) {
  if (currentPage > 1) {
    currentPage--;
    displayPage();
  }
}

void Book::displayPage(void) {
  __display->clearDisplay();
  __display->setCursor(0, 0);
  unsigned int charsPerPage = SCREEN_WIDTH * SCREEN_HEIGHT / (6 * 8);
  unsigned int startIndex = (currentPage - 1) * charsPerPage;
  unsigned int endIndex = startIndex + charsPerPage;

  if (endIndex > strlen(bookContent)) 
  endIndex = strlen(bookContent);
  
  for (int i = startIndex; i < endIndex; i++) 
  __display->write(bookContent[i]);
  
  __display->display();
}

