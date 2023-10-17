//Copyright Grupo 11, Inc. All Rights Reserved.
/***********************************************
 * * * * * * * * * * * * * * * * * * * * * * * *
 * \file
 * Definicion de metodos de: Cursor
 * 
 * * * * * * * * * * * * * * * * * * * * * * * *
***********************************************/
#include "Tools/Cursor.h"

Cursor::Cursor(const std::vector<std::string>& menuOptions, Adafruit_SH1106G& display ,const uint8_t inicialOption ,
const uint8_t MAX_LINE_OPTIONS_OUTPUT,const uint16_t _x ,const uint16_t _y) 
: options(menuOptions), __display(display) ,currentIndex(inicialOption) ,MAX_LINE_OPTIONS_OUTPUT(MAX_LINE_OPTIONS_OUTPUT) ,_x(_x) ,_y(_y){

  totalPages = (options.size() - 1U) / MAX_LINE_OPTIONS_OUTPUT + 1U; // calcular el número total de páginas
  
}

Cursor::Cursor(const std::vector<String>& menuOptions, Adafruit_SH1106G& display ,const uint8_t MAX_LINE_OPTIONS_OUTPUT ,const uint16_t _x ,const uint16_t _y)
: __display(display) ,MAX_LINE_OPTIONS_OUTPUT(MAX_LINE_OPTIONS_OUTPUT) ,_x(_x) ,_y(_y) {

  for(auto it = menuOptions.begin() ; it < menuOptions.end() ; it++)
  options.push_back(std::string(it->c_str()));
  
}

size_t Cursor::getNumberOfOptions() const {
  return options.size();
}

void Cursor::showCurrentPage() {
  xSemaphoreTake( semaphoreDisplay , portMAX_DELAY ); // Bloquear el semáforo
  __display.setTextColor(SH110X_WHITE);
  uint16_t it_algebra = 0U;
  for(auto it_str = options.begin() + currentPage * MAX_LINE_OPTIONS_OUTPUT ; it_str < options.end() &&  it_algebra < MAX_LINE_OPTIONS_OUTPUT; it_str++,it_algebra++ ){
    __display.setCursor(_x, _y + it_algebra * 10 );
    if(currentIndex == std::distance(options.begin() , it_str))
      __display.print(CURSOR_SYMBOL);
    if(CutString){
    __display.print([&it_str](void)->std::string{
      if( it_str->size() > 13U){
        std::string string_result = it_str->substr(0U,12U);
        string_result.append("...");
        return string_result;
      }
      return *it_str;
    }().c_str());
    } else __display.print(it_str->c_str());
    
    
  }

  __display.setCursor( 100 , 56 );
  __display.printf("%2u/%2u" ,currentPage+1U, ceil(options.size() / MAX_LINE_OPTIONS_OUTPUT) );

  __display.flush(); // Flush antes de liberar el mutex
  __display.display();
  xSemaphoreGive( semaphoreDisplay ); // Desbloquear el semáforo

}

void Cursor::clearPage(void){
  xSemaphoreTake( semaphoreDisplay , portMAX_DELAY ); // Bloquear el semáforo
  __display.setTextColor(SH110X_BLACK);

  uint16_t it_algebra = 0U;
  for(auto it_str = options.begin() + currentPage * MAX_LINE_OPTIONS_OUTPUT ; it_str < options.end() &&  it_algebra < MAX_LINE_OPTIONS_OUTPUT; it_str++,it_algebra++ ){
    __display.setCursor(_x, _y + it_algebra * 10 );
    if(currentIndex == std::distance(options.begin() , it_str))
      __display.print(CURSOR_SYMBOL);
    __display.print([&it_str](void)->std::string{
      if( it_str->size() > 13U){
        std::string string_result = it_str->substr(0U,12U);
        string_result.append("...");
        return string_result;
      }
      return *it_str;
    }().c_str());
    
  }

  __display.setCursor( 100 , 56 );
  __display.printf("%2u/%2u" ,currentPage , ceil(options.size() / MAX_LINE_OPTIONS_OUTPUT) );
 
  __display.setTextColor(SH110X_WHITE);
  __display.flush(); // Flush antes de liberar el mutex
  __display.display();
  xSemaphoreGive( semaphoreDisplay ); // Desbloquear el semáforo
}

const char* Cursor::getSelectedOption() {
  __display.clearDisplay();
  clearPage(); //Clear las lineas en las que se mostraran las opciones
  showCurrentPage(); //Mostrar lineas de opciones
  while (true) {
    
    // Boton ENTER Y Boton DERECHA
    if (buttonState(PIN::Buttons::ENTER) == true || buttonState(PIN::Buttons::RIGHT) == true ) {
      delay(DEBOUNCE_TIME);
      clearPage();
      return options[currentIndex].c_str();
    }

    // Boton BACK Y Boton IZQUIERDA
    if (buttonState(PIN::Buttons::BACK) == true || buttonState(PIN::Buttons::LEFT) == true ) {
      delay(DEBOUNCE_TIME);
      clearPage();
      return NULL;
    }

    // Boton UP
    if (buttonState(PIN::Buttons::UP) == true ) {
      clearPage();
      if (currentIndex == currentPage * MAX_LINE_OPTIONS_OUTPUT) {
        if (currentPage == 0U) {
        currentPage = totalPages - 1U;
        } else {
        currentPage--;
        }
        currentIndex = min(options.size(), (currentPage + 1U) * MAX_LINE_OPTIONS_OUTPUT) - 1U;
      } else {
        currentIndex--;
      }
      showCurrentPage();
      delay(DEBOUNCE_TIME);
      
    }

    //Boton DOWN
    if (buttonState(PIN::Buttons::DOWN) == true ) {
      clearPage();
      if(currentIndex == std::distance(options.begin(), options.end())-1){ // -1 porque options.end() devuelve una posicion adelante de la ultima posicion
        currentIndex = 0;
        currentPage = 0;
      }
      else if (currentIndex == (currentPage + 1U) * MAX_LINE_OPTIONS_OUTPUT - 1U) {
        if (currentPage == totalPages - 1U) {
          currentPage = 0;
        } else {
          currentPage++;
        }
        currentIndex = currentPage * MAX_LINE_OPTIONS_OUTPUT;
      } else {
        currentIndex++;
      }

      showCurrentPage();
      delay(DEBOUNCE_TIME);
    }

  }
}

int32_t Cursor::getSelectedOption_number(void){

  __display.clearDisplay();
  clearPage(); //Clear las lineas en las que se mostraran las opciones
  showCurrentPage(); //Mostrar lineas de opciones
  while (true) {

    // Boton ENTER Y Boton DERECHA
    if (buttonState(PIN::Buttons::ENTER) == true || buttonState(PIN::Buttons::RIGHT) == true ) {
      delay(DEBOUNCE_TIME);
      clearPage();
      return currentIndex;
    }

    // Boton BACK Y Boton IZQUIERDA
    if (buttonState(PIN::Buttons::BACK) == true || buttonState(PIN::Buttons::LEFT) == true ) {
      delay(DEBOUNCE_TIME);
      clearPage();
      return -1;
    }

    // Boton UP
    if (buttonState(PIN::Buttons::UP) == true ) {
      clearPage();
      if (currentIndex == currentPage * MAX_LINE_OPTIONS_OUTPUT) {
        if (currentPage == 0U) {
        currentPage = totalPages - 1U;
        } else {
        currentPage--;
        }
        currentIndex = min(options.size(), (currentPage + 1U) * MAX_LINE_OPTIONS_OUTPUT) - 1U;
      } else {
        currentIndex--;
      }
      showCurrentPage();
      delay(DEBOUNCE_TIME);
      
    }

    //Boton DOWN
    if (buttonState(PIN::Buttons::DOWN) == true ) {
      clearPage();
      if(currentIndex == std::distance(options.begin(), options.end())-1){ // -1 porque options.end() devuelve una posicion adelante de la ultima posicion
        currentIndex = 0;
        currentPage = 0;
      }
      else if (currentIndex == (currentPage + 1U) * MAX_LINE_OPTIONS_OUTPUT - 1U) {
        if (currentPage == totalPages - 1U) {
          currentPage = 0;
        } else {
          currentPage++;
        }
        currentIndex = currentPage * MAX_LINE_OPTIONS_OUTPUT;
      } else {
        currentIndex++;
      }
      
      showCurrentPage();
      delay(DEBOUNCE_TIME);
    }

  }

}