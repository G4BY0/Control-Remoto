//Copyright Grupo 11, Inc. All Rights Reserved.
/***********************************************
 * * * * * * * * * * * * * * * * * * * * * * * *
 * \file
 *  ### Cursor.h ### (de desplazamiento de los menus) & Writter (Teclado Virtual Para la escritura de Strings)
 *  - Cursor         ( (BUGS) En Uso, Con un bug de que no se puede ir desde la ultima posicion hacia la primera, Pulsando el boton DOWN)
 *  - CursorUltimate ( (BUGS) En desarrollo y desuso, se penso su uso por problemas que tenia el CursorV2 pero fueron resueltos)
 *  - WritterV2      ( (BUGS) En Uso, bastante estetico pero aun tiene algunos bugs que son como el boton snd que solo funciona cuando el teclado esta en minuscula)
 *  - Writter        ( (BUGS) deprecated)
 * * * * * * * * * * * * * * * * * * * * * * * *
************************************************/
#ifndef CURSOR_H
#define CURSOR_H

#include <Arduino.h> //Arduino Framework
#include <stdint.h> //Tipo de Enteros
#include <inttypes.h> //Tipo de Enteros
#include <Adafruit_SH110X.h> // Lib de display Oled
#include "Interface.h" // Interfaz del usuario

//Cursor
#define MAX_LINE_OPTIONS_OUTPUT 5
#define UP_BUTTON_PIN PIN::Buttons::UP
#define DOWN_BUTTON_PIN PIN::Buttons::DOWN
#define ENTER_BUTTON_PIN PIN::Buttons::ENTER
#define BACK_BUTTON_PIN PIN::Buttons::BACK

//Writter Macros
#define UP_BUTTON_PIN PIN::Buttons::UP
#define DOWN_BUTTON_PIN PIN::Buttons::DOWN
#define LEFT_BUTTON_PIN PIN::Buttons::LEFT
#define RIGHT_BUTTON_PIN PIN::Buttons::RIGHT
#define ENTER_BUTTON_PIN PIN::Buttons::ENTER
#define BACK_BUTTON_PIN PIN::Buttons::BACK

// Oled display size
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Mario's Ideas
// Text input using OLED display an 5 Key Keyboard

/*! @brief Cursor que se desplaza a traves del display*/
class Cursor {
  private:
    std::vector<std::string> options;
    Adafruit_SH1106G* sh1106;
    uint currentIndex;
    uint totalPages;
    uint currentPage;

  public:
    /*! @brief Constructor
        @param menuOptions objeto de caracter std::vector<std::string> que contiene las opciones de trabajo para el cursor
        @param display Recibe referencia o puntero al objeto del display
    */
    Cursor(std::vector<std::string> menuOptions, Adafruit_SH1106G* display);

    /*! @brief Obtiene el numero total strings
        @returns Numero total de punteros que hay dentro del array
    */
    const size_t getNumberOfOptions(void) const;

    /*! @brief Pagina/apartado en el que se encuentra el cursor
        @note Lo muestra en el screen Oled
    */
    void showCurrentPage(void);

    /*! @brief Inicia bucle infinito 
        @returns
        @note Lo muestra en el screen Oled
    */
    const char* getSelectedOption(void);
};

/*! @brief Escritor de Strings Virual en pantalla para el usuario
    \deprecated Se reemplazo por WritterV2, esta en desuso y tiene algunos bugs + errores
    \note Al ser extraido de otra persona este desarrollo y el codigo es bastante inentendible, se hace complicado la optimizacion y correccion de bugs
    - Tiene el problema de que cuando le das por terminado no termina
    - Cuando mantenes el pulsador hacia la derecha se van seleccionando las letras como si hubiera tocado al mismo tiempo ENTER
    - Hay veces que el resaltado se bugea y resalta mas de una letra */
class [[deprecated("Se reemplazo por WritterV2, esta en desuso y tiene errores")]] Writter{

  private:
  //Referencia al objeto del display
    Adafruit_SH1106G& display;
  //Potentiometer PIN A1

  // Variables capturing current and newly calculated position on the letter board (9x3 - 27 postions)
    int New_X=0;
    int Old_X=0;
    int New_Y=0;
    int Old_Y=0;
  // Variable capturing output from Keyboard pin (Values 0 1023)


    boolean Key_pressed=false; 
  // String variable holding the text to transmit
    String To_Transmit="";
  // Length of the text to transmit
    int To_Transmit_Length=0;


  // Used for displaying Leter board
  char Letters[3][10]={"ABCDEFGHI",
                       "JKLMNOPQR",
                       "STUVWXYZ_" };
  
  //! \brief Muestra en pantalla el teclado virtual
  void Graphics(void);

  //! \brief Resalta como un cuadrado en la letra que se encuentra el usuario
  void Highlight_letter(int X, int X_Old, int Y, int Y_Old);

  //! \brief Hace el control de la logica de los pulsadores
  void keyboard(void);


  public:

  //! \brief Constructor
  Writter(Adafruit_SH1106G* display);

  /*! \brief Entra en el sistema del Writter y hasta que el usuario no termine el String entra en Loop
      \returns String terminado
      \note Si el usuario CANCELA o se produce algun problema. Retorna 'nullptr' */
  const char* stringFinished(void);

};

/*! \brief Escritor de Strings Virual en pantalla para el usuario
    \note Reemplaza al Anterior Escritor de Strings ya que es mas estetico y tiene menos bugs.
    Al ser extraido de otra persona este desarrollo y el codigo es bastante inentendible, se hace complicado la optimizacion y correccion de bugs
    Agradecemos de corazon a Mebowfen por el desarrollo! https://github.com/mebowfen/mebowfen_arduino_creations, Video En Youtube: https://www.youtube.com/watch?v=AeB6luzTVfg */
class WritterV2{

  private:
  Adafruit_SH1106G* display_ptr;
  Adafruit_SH1106G& display = *display_ptr;
  ///////////////////////////
  //      -VARIABLES-      //
  ///////////////////////////
  //Data to Send
  struct dataPackage {
    char msg[30];
    int msgLen[1];
  };
  dataPackage data;

  //Pins
  const int leftPin = PIN::Buttons::LEFT;
  const int rightPin = PIN::Buttons::RIGHT;
  const int upPin = PIN::Buttons::UP;
  const int downPin = PIN::Buttons::DOWN;
  const int selectPin = PIN::Buttons::ENTER;
  const int switchPin = 7;

  //timeDelay
  int buttonDelay = 0;
  int selectDelay = 0;
  int radioDelay = 0;
  int blinkDelay = 0;

  //drawMenu
  bool show = true;
  bool letters = true;
  bool capitalize = false;

  //drawCursor
  int x = 0;
  int y = 33;
  int width = 7;
  int height = 11;
  bool select = false;
  bool transmit = false;

  //drawText
  int index = 0;
  //Vector de longitud fija en el que el usuario escribira el String pedido
  char msgToSend[26] = {'\0'}; //Inicializa todos los miembros como EOF
  int num = 0;

  // - Creo que esto se podria eliminar porque no tiene uso alguno
  //receiveText
  int msgFlag = 0;
  char convertedMsg[30];
  bool displayMsg = false;
  bool ringFlag = true;
  bool booleanStringFinished = false;

  //drawBlink (Parpadeo de la letra a continuacion del String)
  int xBlink = 0;
  int yBlink1 = 0;
  int yBlink2 = 0;

  public:

  //! \brief Constructor
  WritterV2(Adafruit_SH1106G* displayReference);

  /*! \brief Hace en conjunto la interfaz del Writter para el usuario */
  void atcLoop(void);

  /*! \brief Logica de calculo para el rebote de los pulsadores
      \note NO USA PARADAS DEL PROCESADOR!!! */
  void timeDelay(void);

  /*! \brief Dibuja en pantalla el comodo teclado virtual para el usuario */
  void drawMenu(void);

  /*! \brief Dibuja en pantalla el cursor dentro del teclado virtual */
  void drawCursor(void);

  /*! \brief Dibuja en pantalla el texto que se esta escribiendo */
  void drawText(void);

  /*! \brief dibuja en pantalla el blink de donde se deberia escribir la siguiente letra */
  void drawBlink(void);

  /*! \brief Proceso de seleccion para el usuario
      \return String seleccionado
      \note Si se cancela la seleccion, retorna 'nullptr' */
  const char* stringFinished(void);

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