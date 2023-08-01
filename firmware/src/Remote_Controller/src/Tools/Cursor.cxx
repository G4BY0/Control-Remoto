//Copyright Grupo 11, Inc. All Rights Reserved.
/***********************************************
 * * * * * * * * * * * * * * * * * * * * * * * *
 * \file
 * Definicion de metodos de: Cursor
 * 
 * * * * * * * * * * * * * * * * * * * * * * * *
***********************************************/
#include "Tools/Cursor.h"

const char* CursorUltimate::getOption( std::vector<std::string> Strings , const size_t CantidadDeStrings , Adafruit_SH1106G* OLEDObject_ptr ){

  Adafruit_SH1106G& Cout = *OLEDObject_ptr;
  const int MAX_LINE_OPTIONS_OUTPUT = 5U;
  Cout.clearDisplay();
  Cout.setTextSize(1);
  Cout.setTextColor(SH110X_WHITE);

  Cout.setCursor(0,0);
  Cout.println(F("Elegir Opcion..."));
  Cout.display();

  //Luego editar porque esta variable solamente esta pensada si: MAX_LINE_OPTIONS_OUTPUT == 3, SINO GENERARA BUGS, CUIDADO!
  const uint8_t lineUpStrings_Y [MAX_LINE_OPTIONS_OUTPUT] = { 0, 15, 30 }; //Coordenada
  const uint8_t && lineUPStrings_X = 10; //Coordenada

  const uint8_t && lineUPcursorSymbol_X = 5; //Coordenada
  #define CURSOR_SYMBOL '>'

  const uint8_t && totalNumberOfOptions = sizeof(Strings) / sizeof(Strings[0U]); //Total De Opciones

  //Debugging
  Serial.println(sizeof(Strings) / sizeof(Strings[0U]));
  const uint8_t && totalPages = ceil((totalNumberOfOptions % MAX_LINE_OPTIONS_OUTPUT)); //Total de paginas

  //Inicializacion del Cursor
  uint8_t cursorPage = 0;
  uint8_t cursorString = 0;

  //En caso de querer aumentar el performance... se podria hacer con una matriz en vez de dos variables para el manejo del cursor (cursorPage, CursorString)

  //Mostrando Screen De Primeras
  Cout.clearDisplay();
  for( uint8_t iterator = 0 ; iterator < MAX_LINE_OPTIONS_OUTPUT ; iterator++ ){

    Cout.setCursor( lineUPStrings_X , lineUpStrings_Y[iterator] );
    Cout.println( Strings[(cursorPage * MAX_LINE_OPTIONS_OUTPUT) + iterator].c_str() );

  }
  Cout.setCursor( lineUPcursorSymbol_X , lineUpStrings_Y[cursorString] ); 
  Cout.print( '>' );
  Cout.display();

  for(;;){

    if ( buttonState(PIN::Buttons::UP) == true ) {

      //Si llegara a cambiar de Pagina...
      if ( (cursorPage == 0U) && (cursorString == 0U) ) {

        cursorPage = totalPages;
        cursorString = totalNumberOfOptions;

        Cout.clearDisplay();
        for( uint8_t iterator = 0 ; iterator < MAX_LINE_OPTIONS_OUTPUT ; iterator++ ){

          Cout.setCursor( lineUPStrings_X , lineUpStrings_Y[iterator] );
          Cout.println( Strings[(cursorPage * MAX_LINE_OPTIONS_OUTPUT) + iterator].c_str() );

        }
        Cout.setCursor( lineUPcursorSymbol_X , lineUpStrings_Y[cursorString] ); 
        Cout.print( '>' );
        Cout.display();

      } 

      //Si llegara a cambiar de Pagina...
      else if ( cursorString == 0U ){  // (cursorString % MAX_LINE_OPTIONS_OUTPUT) == 0 Condicional picante se me habia ocurrido

        cursorPage--;
        cursorString = MAX_LINE_OPTIONS_OUTPUT;

        //Hago un Refresh para mostrar la actual pagina
        Cout.clearDisplay();
        for( uint8_t iterator = 0 ; iterator < MAX_LINE_OPTIONS_OUTPUT ; iterator++ ){

          Cout.setCursor( lineUPStrings_X , lineUpStrings_Y[iterator] );
          Cout.println( Strings[(cursorPage * MAX_LINE_OPTIONS_OUTPUT) + iterator].c_str() );

        }
        Cout.setCursor( lineUPcursorSymbol_X , lineUpStrings_Y[cursorString] ); 
        Cout.print( '>' );
        Cout.display();
        
      } else {

        //Borro la opcion que estaba marcada anteriormente antes de haber pulsado
        Cout.setCursor( lineUPcursorSymbol_X , lineUpStrings_Y[cursorString] ); 
        Cout.print( ' ' );
        Cout.display();

        //Escribo el Cursor en la opcion correspondiente
        cursorString--;
        Cout.setCursor( lineUPcursorSymbol_X , lineUpStrings_Y[cursorString] ); 
        Cout.print( '>' );
        Cout.display();

      }
      delay(DEBOUNCE_TIME);
    }

    //Si llegara a cambiar de Pagina...
    if ( buttonState(PIN::Buttons::DOWN) == true ) {

      //Si llegara a cambiar de Pagina...
      if ( (cursorPage == totalPages) && (cursorString == (MAX_LINE_OPTIONS_OUTPUT-1) ) ) {

        cursorPage = 0;
        cursorString = 0;

        Cout.clearDisplay();
        for( uint8_t iterator = 0 ; iterator < MAX_LINE_OPTIONS_OUTPUT ; iterator++ ){

          Cout.setCursor( lineUPStrings_X , lineUpStrings_Y[iterator] );
          Cout.println( Strings[(cursorPage * MAX_LINE_OPTIONS_OUTPUT) + iterator].c_str() );

        }
        Cout.setCursor( lineUPcursorSymbol_X , lineUpStrings_Y[cursorString] ); 
        Cout.print( '>' );
        Cout.display();

      }

      //Si llegara a cambiar de Pagina...
      else if( cursorString == (MAX_LINE_OPTIONS_OUTPUT-1) ){ 

        cursorPage++;
        cursorString = 0;

        //Hago un Refresh para mostrar la actual pagina
        Cout.clearDisplay();
        for( uint8_t iterator = 0 ; iterator < MAX_LINE_OPTIONS_OUTPUT ; iterator++ ){

          Cout.setCursor( lineUPStrings_X , lineUpStrings_Y[iterator] );
          Cout.println( Strings[(cursorPage * MAX_LINE_OPTIONS_OUTPUT) + iterator].c_str() );

        }
        Cout.setCursor( lineUPcursorSymbol_X , lineUpStrings_Y[cursorString] ); 
        Cout.print( '>' );
        Cout.display();

      } else {

        //Borro la opcion que estaba marcada anteriormente antes de haber pulsado
        Cout.setCursor( lineUPcursorSymbol_X , lineUpStrings_Y[cursorString] ); 
        Cout.print( ' ' );
        Cout.display();

        //Escribo el Cursor en la opcion correspondiente
        cursorString++;
        Cout.setCursor( lineUPcursorSymbol_X , lineUpStrings_Y[cursorString] ); 
        Cout.print( '>' );
        Cout.display();

      }
      delay(DEBOUNCE_TIME);
    }

    //Si se presiona boton Enter
    if ( buttonState(PIN::Buttons::ENTER) == true ){
    delay(DEBOUNCE_TIME);
    return Strings[ ( totalPages * MAX_LINE_OPTIONS_OUTPUT ) + cursorString ].c_str();
    }
    //Si se presiona el boton Back
    if ( buttonState(PIN::Buttons::BACK) == true ){
    delay(DEBOUNCE_TIME);
    return nullptr; //Se cancelo la seleccion
    }
  }

}


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
  /*
  for (uint16_t iterator = currentPage * MAX_LINE_OPTIONS_OUTPUT; 
    iterator < options.size();
    iterator++
    ) {
    __display.setCursor(0, (iterator - currentPage * MAX_LINE_OPTIONS_OUTPUT + 1U) * 10U);
    if (iterator == currentIndex) __display.print(CURSOR_SYMBOL); // Si la opcion actual se encuentra el cursor, dibujarlo
    __display.print([string = options[iterator]](void)->std::string{
      
      if( string.size() > 13U){
        std::string string_result = string.substr(0U,12U);
        string_result.append("...");

        return string_result;
      }
      return string;

    }().c_str());
    
  }
  */

  __display.setCursor( 100 , 56 );
  __display.printf("%2u/%2u" ,currentPage , ceil(options.size() / MAX_LINE_OPTIONS_OUTPUT) );

  __display.display();
  __display.flush(); // Flush antes de liberar el mutex
  xSemaphoreGive( semaphoreDisplay ); // Desbloquear el semáforo

}

void Cursor::clearPage(void){
  xSemaphoreTake( semaphoreDisplay , portMAX_DELAY ); // Bloquear el semáforo
  __display.clearDisplay();
  for(uint8_t iterator = 0U; iterator < MAX_LINE_OPTIONS_OUTPUT; iterator++){
    __display.setCursor( 0 , iterator * 10U );
    __display.println();
  }
  __display.display();
  xSemaphoreGive( semaphoreDisplay ); // Desbloquear el semáforo
}

const char* Cursor::getSelectedOption() {
  clearPage(); //Clear las lineas en las que se mostraran las opciones
  showCurrentPage(); //Mostrar lineas de opciones
  while (true) {
    // Boton ENTER Y Boton DERECHA
    if (buttonState(PIN::Buttons::ENTER) == true || buttonState(PIN::Buttons::RIGHT) == true ) {
      delay(DEBOUNCE_TIME);
      return options[currentIndex].c_str();
    }
    // Boton BACK Y Boton IZQUIERDA
    if (buttonState(PIN::Buttons::BACK) == true || buttonState(PIN::Buttons::LEFT) == true ) {
      delay(DEBOUNCE_TIME);
      return nullptr;
    }
    // Boton UP
    if (buttonState(PIN::Buttons::UP) == true ) {
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
      clearPage();
      showCurrentPage();
      delay(DEBOUNCE_TIME);
      
    }
    //Boton DOWN
    if (buttonState(PIN::Buttons::DOWN) == true ) {
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
      /*
      if(currentIndex > ((currentPage == 0 ? 1 : currentPage) * MAX_LINE_OPTIONS_OUTPUT)-1){
        currentPage++;
      }
      if(currentIndex == std::distance(options.begin(), options.end())-1 ){ // -1 porque options.end() devuelve una posicion adelante de la ultima posicion
        currentIndex = 0;
        currentPage = 0;
      } else {
        currentIndex++;
      }
      */
      clearPage();
      showCurrentPage();
      delay(DEBOUNCE_TIME);
    }
  }
}