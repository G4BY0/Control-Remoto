#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#define TFT_DC 9
#define TFT_CS 10

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  tft.begin();

  tft.setCursor(26, 120);
  tft.setTextColor(ILI9341_RED);
  tft.setTextSize(3);
  tft.println("Hello, TFT!");

  tft.setCursor(20, 160);
  tft.setTextColor(ILI9341_GREEN);
  tft.setTextSize(2);
  tft.println("I can has colors?");

  tft.fillScreen(0x00); // PONGO LA PANTALLA EN NEGRO COMO PARA CLEREAR EL DISPLAY

  tft.setCursor(26, 120);
  tft.setTextColor(ILI9341_RED);
  tft.setTextSize(3);
  tft.println("Prendiendo");

  tft.setCursor(20, 160);
  tft.setTextColor(ILI9341_BLUE);
  tft.setTextSize(2);
  tft.println("Proyecto pensado \n y fabricado por:");

  tft.setCursor(100, 200);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(1);
  tft.println("Gabriel Guereta");

  tft.setCursor(100, 220);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(1);
  tft.println("Joaquin Torres");

  tft.setCursor(100, 240);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(1);
  tft.println("Melvin Schwartzbart");


}

void loop() {
  
if(SD.begin()){

  Serial.println("Memoria SD detectada");

  tft.setCursor(70, 20);
  tft.setTextColor(ILI9341_RED);
  tft.setTextSize(1);
  tft.println("Memoria SD detectada");

  Serial.println("  Crear Perfil");
    
  if(!Serial.available()){


  }
  //LUEGO AGREGAR SI QUIERO BORRAR UN PERFIL



  }
  else {

    Serial.println("Ingrese memoria SD");

    tft.setCursor(70, 20);
    tft.setTextColor(ILI9341_RED);
    tft.setTextSize(1);
    tft.println("Ingrese memoria SD");

  }


}