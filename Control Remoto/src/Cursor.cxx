//Copyright Grupo 7, Inc. All Rights Reserved.
/***********************************************
 * * * * * * * * * * * * * * * * * * * * * * * *
 * \file
 * Definicion de metodos de las Clases: CursorUltimate (En desarrollo y desuso, se penso su uso por problemas que tenia el CursorV2 pero fueron resueltos)
 *                                      Cursor (en Uso, Con un bug de que no se puede ir desde la ultima posicion hacia la primera, Pulsando el boton DOWN)
 *                                      Writter (deprecated)
 *                                      WritterV2 (en Uso, bastante estetico pero aun tiene algunos bugs que son como el boton snd que solo funciona cuando el teclado esta en minuscula)
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

//Si recibo la cadena de strings en formato std::vector<std::string>
Cursor::Cursor(std::vector<std::string> menuOptions, Adafruit_SH1106G* display) : options(menuOptions), sh1106(display), currentIndex(0), currentPage(0) {
  totalPages = (getNumberOfOptions() - 1) / MAX_LINE_OPTIONS_OUTPUT + 1; // calcular el número total de páginas
}

const size_t Cursor::getNumberOfOptions() const {
  return options.size();
}

void Cursor::showCurrentPage() {
  sh1106->clearDisplay();
  sh1106->setCursor(0, 0);
  sh1106->print(F("Seleccione una opcion:"));
  for (uint8_t iterator = currentPage * MAX_LINE_OPTIONS_OUTPUT; iterator < min(getNumberOfOptions(), (currentPage + 1) * MAX_LINE_OPTIONS_OUTPUT) && iterator < options.size(); iterator++) {
    sh1106->setCursor(0, (iterator - currentPage * MAX_LINE_OPTIONS_OUTPUT + 1) * 10);
    if (iterator == currentIndex) {
      sh1106->print(">");
    }
    sh1106->print(options[iterator].c_str());
  }
  sh1106->display();
}

const char* Cursor::getSelectedOption() {
  while (true) {
    showCurrentPage();
    // Ajuste en la función getSelectedOption()
    if (buttonState(UP_BUTTON_PIN) == HIGH) {
      if (currentIndex == currentPage * MAX_LINE_OPTIONS_OUTPUT) {
        if (currentPage == 0) {
        currentPage = totalPages - 1;
        } else {
        currentPage--;
        }
        currentIndex = min(getNumberOfOptions(), (currentPage + 1) * MAX_LINE_OPTIONS_OUTPUT) - 1;
      } else {
        currentIndex--;
      }
      delay(DEBOUNCE_TIME);
      showCurrentPage();
    }
    //Boton DOWN
    if (buttonState(DOWN_BUTTON_PIN) == HIGH) {
      if (currentIndex == (currentPage + 1) * MAX_LINE_OPTIONS_OUTPUT - 1) {
        if (currentPage == totalPages - 1) {
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

    //Boton ENTER
    if (buttonState(ENTER_BUTTON_PIN) == HIGH) {
      delay(DEBOUNCE_TIME);
      return options[currentIndex].c_str();;
    }
    //Boton BACK
    if (buttonState(BACK_BUTTON_PIN) == HIGH) {
      delay(DEBOUNCE_TIME);
      return nullptr;
    }
  }
}

Writter::Writter(Adafruit_SH1106G* object) : display(*object){}

void Writter::Graphics(void){
  
  // Clear the buffer
  display.clearDisplay();
  display.display();
  
  // Display filled in rect in the top section of the display when To_Transfer would be output
  display.fillRect(0, 0, 128, 15, SH110X_INVERSE);
  display.drawRect(110, 2, 16, 12, SH110X_BLACK);
  display.setTextSize(1);            
  display.setTextColor(SH110X_BLACK);  
  display.setCursor(113,4); 
  display.println(F("OK"));
  display.display();
  // Display Letter Board 3 rows 9 character in each row 
  display.setTextSize(2);            
  display.setTextColor(SH110X_WHITE);       
  for (int j=0; j<3;j++){
    for (int i=0; i<9;i++){
      display.setCursor(i*12+2*i+1,j*16+17);           
      display.println(Letters[j][i]);
      delay(10);
      display.display();
    }
  }
  // Highlight character A by displaying Inverse rect at first position
  display.fillRect(0, 16, 12, 16, SH110X_INVERSE);
  display.display();
}

void Writter::Highlight_letter(int X, int X_Old, int Y, int Y_Old){

  // When position changes 
  // Draw the inverse rect in the Old_pos to deactivate  the highlight in the old spot
  // Draw the inverse rect to Highlite the new spot
  // Displaying Inverse rect in a new position to highlight
  display.fillRect(X*12+2*X, Y*16 +16, 12, 16, SH110X_INVERSE);
  // Displaying Inverse rect in the old positon to unhighlight
  display.fillRect(X_Old*12+2*X_Old, Y_Old*16 +16, 12, 16, SH110X_INVERSE);
  display.display();

}

void Writter::keyboard(void) {
  
  while(INFINITE_LOOPING){

    Key_pressed=true;
    if ( buttonState(PIN::Buttons::BACK)  == HIGH )               {To_Transmit = ""; break;}
    if ((buttonState(PIN::Buttons::LEFT)  == HIGH ) and Old_X>0)  { New_X=Old_X-1; delay(DEBOUNCE_TIME);}
    if ((buttonState(PIN::Buttons::RIGHT) == HIGH ) and Old_X<9)  { New_X=Old_X+1; delay(DEBOUNCE_TIME);}
    if ((buttonState(PIN::Buttons::UP)    == HIGH ) and Old_Y>-1) { New_Y=Old_Y-1; delay(DEBOUNCE_TIME);}
    if ((buttonState(PIN::Buttons::DOWN)  == HIGH ) and  Old_Y<2 ){ New_Y=Old_Y+1; delay(DEBOUNCE_TIME);}
    if (buttonState(PIN::Buttons::RIGHT)  == HIGH ) {                              delay(DEBOUNCE_TIME);
     if (New_Y!=-1){
      To_Transmit=To_Transmit + Letters[New_Y][New_X];
      To_Transmit_Length++;
      display.setTextSize(1);
      display.setCursor(3,1);
      display.setTextColor(SH110X_BLACK);
      display.fillRect(0, 0, 100, 15, SH110X_WHITE);
      display.println(To_Transmit);
      display.display();
     }
     else{
      for (int i=1;i<9;i++) {
        display.fillRect(0, 0, 128, 15, SH110X_INVERSE);
        delay(DEBOUNCE_TIME);
        display.display();
      }
     }
    }     
    if (New_Y==-1 and Old_Y==0){
      display.fillRect(110, 2, 16, 12, SH110X_INVERSE); 
      display.fillRect(Old_X*12+2*Old_X, Old_Y*16 +16, 12, 16, SH110X_INVERSE);
      
    }
    if (New_Y==0 and Old_Y==-1){
      display.fillRect(110, 2, 16, 12, SH110X_INVERSE); 
      display.fillRect(New_X*12+2*New_X, New_Y*16 +16, 12, 16, SH110X_INVERSE);
      Old_X=New_X;
      Old_Y=New_Y;;
      
    }
    if ((Old_X!=New_X or Old_Y!=New_Y) and Old_Y!=-1 ){
      if (New_Y!=-1 )Highlight_letter (New_X,Old_X,New_Y,Old_Y);
      Old_X=New_X;
      Old_Y=New_Y;
    }

    display.display();
  }

}

const char* Writter::stringFinished(void){ 
  Writter::Graphics();
  Writter::keyboard();
  if(To_Transmit.c_str() == "")
    return nullptr;
  return To_Transmit.c_str(); 
}
