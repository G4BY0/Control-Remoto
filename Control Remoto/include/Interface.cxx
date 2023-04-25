//Copyright Grupo 7, Inc. All Rights Reserved.
/***********************************************
 * * * * * * * * * * * * * * * * * * * * * * * *
 * \file
 * Source code of members for Interface.h
 * 
 * * * * * * * * * * * * * * * * * * * * * * * *
***********************************************/

#include "Interface.h"


void buttonsBegin(void){

  pinMode(PIN::Buttons::UP,     INPUT);
  pinMode(PIN::Buttons::DOWN,   INPUT);
  pinMode(PIN::Buttons::LEFT ,  INPUT);
  pinMode(PIN::Buttons::RIGHT , INPUT);
  pinMode(PIN::Buttons::BACK ,  INPUT);
  pinMode(PIN::Buttons::ENTER,  INPUT);  

}

inline bool buttonState(const uint8_t PIN_BUTTON) { 
  return (digitalRead(PIN_BUTTON)); 
}


void displayBegin(void){

  display.begin(I2C_ADDRESS, true);
  display.display();
  display.clearDisplay();
  display.display();

}

uint8_t Interface::hub(void){

  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  
  display.setCursor(LINE_STRING_X,LINE_STRING_Y[0]);
  display.print    ("  PROFILES"   );

  display.setCursor(LINE_STRING_X,LINE_STRING_Y[1]);
  display.print    ("  ADD PROFILE");

  display.setCursor(LINE_STRING_X,LINE_STRING_Y[2]);
  display.print    ("  DELETE PROFILE");

  display.display();

  Cursor cursor(3);
  return cursor.options();
  

}

void Interface::profiles(void){

  
const int button_up_pin = 2;
const int button_down_pin = 3;

const int max_lines = 5; // Máximo número de líneas que se pueden mostrar en pantalla
int current_page = 0; // Página actual del texto
int num_pages = 0; // Número total de páginas del texto
int current_line = 0; // Línea actual del texto

  // ---------------------------------------------------------------------
  const String* arrayProfilesName = Profiles::showProfiles_();
  // Leer el estado de los botones
  bool button_up = digitalRead(button_up_pin) == LOW;
  bool button_down = digitalRead(button_down_pin) == LOW;

  // Mostrar el texto en pantalla
  const String* text_ptr = Profiles::showProfiles_(); // getTextPointer();
  
  
  if (text_ptr == nullptr) {
    Serial.println("No hay perfiles disponibles en la SD O hubo una obstruccion al intentar hacerlo...");
    return; // El puntero es nulo, salir de la función
    
  }

  int num_lines = countLines(text_ptr);
  num_pages = num_lines / max_lines;
  if (num_lines % max_lines != 0) {
    num_pages++; // Añadir una página adicional para mostrar las líneas restantes
  }

  display.clearDisplay();
  for (int i = current_page*max_lines; i < (current_page+1)*max_lines; i++) {
    if (i >= num_lines) {
      break; // Se han mostrado todas las líneas, salir del bucle
    }

    const String& line = text_ptr[i];
    display.setCursor(0, (i - current_page*max_lines)*10);
    display.println(line);
  }
  display.display();

  // Actualizar la posición del cursor basado en los botones
  if (button_up && current_line > 0) {
    current_line--;
  }
  else if (button_down && current_line < max_lines - 1 && current_page*max_lines + current_line < num_lines - 1) {
    current_line++;
  }

  // Actualizar la página del texto si es necesario
  if (current_line == 0 && button_up && current_page > 0) {
    current_page--;
  }
  else if (current_line == max_lines - 1 && button_down && current_page < num_pages - 1) {
    current_page++;
  }
  //-------------------------------------------------------

  /* 
  if(arrayProfilesName == nullptr){

    Serial.println("No hay perfiles disponibles en la SD O hubo una obstruccion al intentar hacerlo...");
    return;

  } else {

    Serial.println("Mostrando perfiles:");
    display.

  } 
  */


}

void Interface::addProfile(void){


  Cursor cursor();
  Profiles::createProfile_(cursor.write_ptr());
  //String name = cursor.writer_ptr(); DESARROLLAR ESTA PARTE IGUAL...
  //Profiles::createProfile_(cursor.write_ptr());

}

void Interface::deleteProfile(void){

  Profiles::showProfiles_(); 

  Cursor cursor();
  Profiles::createProfile_(cursor.write_ptr());

  // if(/*PONER AQUI OPCIONES ONDA .txt que hay en el directorio*/ == 0){ Interface::nonProfiles() } else {continua} (DESARROLLAR IGUAL, HAY Q PENSARLO MAS A FONDO ESTA PARTE, PARA PODER VOLVER DIGO)
 

}


//PARTE DE LO DE SHOWPROFILES
// Función que devuelve un puntero al texto a mostrar
const String* getTextPointer() {
  static const String text[] = {"Cadena 1", "Cadena 2", "Cadena 3", "Cadena 4", "Cadena 5", "Cadena 6", "Cadena 7", "Cadena 8", "Cadena 9", "Cadena 10"};
  const String* ptr = text;
  return ptr;
}

// Función que cuenta el número de líneas en el texto
int countLines(const String* text) {
  int count = 0;
  while (text[count].length() > 0) {
    count++;
  }
  return count;
}