//Copyright Grupo 11, Inc. All Rights Reserved.
/***********************************************
 * * * * * * * * * * * * * * * * * * * * * * * *
 * \file
 * Definicion de metodos de: CursorUltimate & Cursor
 * 
 * * * * * * * * * * * * * * * * * * * * * * * *
***********************************************/
#include "Cursor.h"

const char* CursorUltimate::getOption( std::vector<std::string> Strings , const size_t CantidadDeStrings , Adafruit_SH1106G* OLEDObject_ptr ){

  Adafruit_SH1106G& Cout = *OLEDObject_ptr;

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
  const uint8_t && totalPages = ceil ((totalNumberOfOptions % MAX_LINE_OPTIONS_OUTPUT)); //Total de paginas

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


Cursor::Cursor( std::vector<std::string> menuOptions, Adafruit_SH1106G* display ) 
: options(menuOptions), sh1106(display), currentIndex(0), currentPage(0) {
  totalPages = (getNumberOfOptions() - 1U) / MAX_LINE_OPTIONS_OUTPUT + 1U; // calcular el número total de páginas
}

//Si recibo la cadena de strings en formato std::vector<std::string>
Cursor::Cursor(std::vector<std::string> menuOptions, Adafruit_SH1106G* display , const uint8_t inicialOption ) 
: options(menuOptions), sh1106(display), currentIndex(inicialOption), currentPage(0) {
  totalPages = (getNumberOfOptions() - 1U) / MAX_LINE_OPTIONS_OUTPUT + 1U; // calcular el número total de páginas
}


const size_t Cursor::getNumberOfOptions() const {
  return options.size();
}

void Cursor::showCurrentPage() {

  // Bloquear el semáforo
  xSemaphoreTake( semaphoreDisplay , portMAX_DELAY );

  sh1106->clearDisplay();
  sh1106->setCursor(0, 0);
  sh1106->print(F("Seleccione una opcion:"));
  for (uint8_t iterator = currentPage * MAX_LINE_OPTIONS_OUTPUT; iterator < min(getNumberOfOptions(), (currentPage + 1U) * MAX_LINE_OPTIONS_OUTPUT) && iterator < options.size(); iterator++) {
    sh1106->setCursor(0, (iterator - currentPage * MAX_LINE_OPTIONS_OUTPUT + 1U) * 10U);
    if (iterator == currentIndex) {
      sh1106->print(">");
    }
    sh1106->print(options[iterator].c_str());
  }
  sh1106->display();

  // Desbloquear el semáforo
  xSemaphoreGive( semaphoreDisplay );

}

const char* Cursor::getSelectedOption() {
  while (true) {
    showCurrentPage();
    // Boton ENTER Y Boton DERECHA
    if (buttonState(ENTER_BUTTON_PIN) == true || buttonState(PIN::Buttons::RIGHT) == true ) {
      delay(DEBOUNCE_TIME);
      return options[currentIndex].c_str();
    }
    // Boton BACK Y Boton IZQUIERDA
    if (buttonState(BACK_BUTTON_PIN) == true || buttonState(PIN::Buttons::LEFT) == true ) {
      delay(DEBOUNCE_TIME);
      return nullptr;
    }
    // Boton UP
    if (buttonState(UP_BUTTON_PIN) == true ) {
      if (currentIndex == currentPage * MAX_LINE_OPTIONS_OUTPUT) {
        if (currentPage == 0U) {
        currentPage = totalPages - 1U;
        } else {
        currentPage--;
        }
        currentIndex = min(getNumberOfOptions(), (currentPage + 1U) * MAX_LINE_OPTIONS_OUTPUT) - 1U;
      } else {
        currentIndex--;
      }
      delay(DEBOUNCE_TIME);
      showCurrentPage();
    }
    //Boton DOWN
    if (buttonState(DOWN_BUTTON_PIN) == true ) {
      if (currentIndex == (currentPage + 1U) * MAX_LINE_OPTIONS_OUTPUT - 1U) {
        if (currentPage == totalPages - 1U) {
          currentPage = 0;
        } else {
          currentPage++;
        }
        currentIndex = currentPage * MAX_LINE_OPTIONS_OUTPUT;
      } else {
        currentIndex++;
      }
      delay(DEBOUNCE_TIME);
      showCurrentPage();
    }
  }
}