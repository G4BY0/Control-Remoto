
#ifndef WRITTER_H
#define WRITTER_H

#include <cstdint>
#include <Adafruit_SH110X.h>

#include "System.h" // Para usar el mutex del display
#include "PIN.h"   // Para tener manejo de los pulsadores

//Pulsadores
/*! \brief Estado logico de la botonera
  \param PIN_BUTTON PIN de la placa conectada al boton
  \note Instalacion PullUP
  \returns Si esta presionado 'true', sino 'false' */
#define buttonState(PIN_BUTTON) !digitalRead(PIN_BUTTON) 

#define DEBOUNCE_TIME 500 //Tiempo de seguridad por el rebote mecanico de los pulsadores

/*! \brief Escritor de Strings Virtual en pantalla para el usuario
    \note Los botones se usan para el desplazamiento.
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
  const uint8_t switchPin = 7;

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
  char msgToSend[30];
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

  /*! \brief Logica de calculo para el rebote de los pulsadores */
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

#endif //WRITTER_H