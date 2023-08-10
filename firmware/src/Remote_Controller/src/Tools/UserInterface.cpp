#include "Tools/UserInterface.h"

void UI_t::batteryPercentage(char* buffer){
  /* (DEBUGGING AND DEVELOPING) - FEATURE: SIMBOLS 
  // Carácter Unicode 🔋 (U+1F50B) en un wchar_t
  constexpr wchar_t batteryFull_unicodeChar = 0x1F50B;

  // Carácter Unicode ⚡ (U+26A1) en un wchar_t
  constexpr wchar_t charging_unicodeChar = 0x26A1;

  // Carácter Unicode 🔌 (U+1F50C) en un wchar_t
  constexpr wchar_t batteryLow_unicodeChar = 0x1F50C;

  const uint8_t battery_percentage = map(analogRead(PIN::Energy::BATTERY), 0l , 4095l , 0l , 100l );

  // Convertir el número a una cadena wchar_t*
  std::wstring battery_percentage_wstr = std::to_wstring(battery_percentage);

  */
  /* (DEBUGGING AND DEVELOPING) - FEATURE: SIMBOLS 
  battery_percentage_wstr += battery_percentage > 20 ? batteryFull_unicodeChar : batteryLow_unicodeChar;

  // Copiar la cadena de wstr al búfer buffer usando wcscpy
  wcscpy(buffer, battery_percentage_wstr.c_str());

  // Convertir el número a una cadena wchar_t*
  swprintf(buffer, sizeof(buffer) / sizeof(wchar_t), L"%ld", battery_percentage_wstr);
  */

  sprintf(buffer, "%ld%%" ,map(analogRead(PIN::Energy::BATTERY), 0l , 4095l , 0l , 100l ));
  

}

void UI_t::graphic(UI_position manifest){
  buffer << [&]() -> std::string {
    std::string _spaces;
    std::string buff = buffer.str();
    for(std::string::iterator it = buff.begin(); it != buff.end(); it++)
      buffer << " ";
    return _spaces;
  }();
  display.setTextColor(SH110X_BLACK);
  xSemaphoreTake( semaphoreDisplay , portMAX_DELAY ); // Bloquear el semáforo
  switch (manifest){
    
  case UI_position::UPPERLEFT_CORNER:
    display.setCursor(0,0);
    display.print(buffer.str().c_str());
    display.setCursor(0,0);
    break;

  case UI_position::UPPERRIGHT_CORNER:
    display.setCursor(58,0);
    display.print(buffer.str().c_str());
    display.setCursor(58,0);
    // (dev) Aun no testeado
    break;

  case UI_position::LOWERLEFT_CORNER:
    display.setCursor(0,58);
    display.print(buffer.str().c_str());
    display.setCursor(0,58);
    break;

  case UI_position::LOWERRIGHT_CORNER:
    display.setCursor(58,58);
    display.print(buffer.str().c_str());
    display.setCursor(58,58);
    // (dev) Aun no testeado
    break;
  
  }
  display.setTextColor(SH110X_WHITE);
  buffer.str("");
  //Tiempo
  if(time_status == true){
      time_RTC = RTC.getTimeStruct(); // Sincronizo el tiempo a mostrar con el clock del sistema
      char clock_str[13] = {'\0'};
      Interface::clock(time_RTC, clock_str);
      buffer << clock_str;
      buffer << " ";
  }
  //Nivel de bateria en porcentaje
  if(battery_status == true){
    /*(DEV) - FEATURE: SIMBOLS 
    wchar_t* battery_str[5] = {'\0'};*/
    char battery_str[5] = {'\0'};
    batteryPercentage(battery_str);
    buffer << battery_str;
  }
  display.print(buffer.str().c_str());
  bool wifi_current_status = WiFi.status();
  display.setFont(&icons_UI);
  if(wifi_current_status == WL_CONNECTED){ //When WiFi is conected
    auto wifi_signal = map(WiFi.RSSI(), 0, -127, 1, 5);
    switch (wifi_signal){
      case 1: display.print(36); break;
      case 2: display.print(37); break;
      case 3: display.print(38); break;
      case 4: display.print(39); break;
      case 5: display.print(40); break;
    }
  }
  else display.print(49); // For disconnected
  display.setFont(NULL);

  display.flush();
  display.display();
  xSemaphoreGive(semaphoreDisplay); // Desbloquear el semáforo

}

void UI_t::run(void){

  //Task Asincronico para el UI
  xTaskCreate(
    UI_async,                   //Funcion codigo del Task
    "Task_UI",                  //Nombre del Task 
    3000U,                      //Reserva de espacio en la Pila 
    NULL,                       //Argumentos
    tskIDLE_PRIORITY,           //Prioridad
    &handle                     //Handle   
  );

} 

void UI_async(void* nonParameters){
  buffer.str("");
  while(1){
    if(UI.show == true)
      UI.graphic();
    
    // Pausar la tarea durante un breve periodo de tiempo
    vTaskDelay(pdMS_TO_TICKS(1U));
  }
  vTaskDelete(NULL);
}

void UI_t::stop(void){
  vTaskDelete(handle);
}

UI_t::UI_t(void){ }
UI_t::~UI_t(void){ }

UI_t UI;

/*
// Four tiles for a battery icon of 16x16.
  // The bitmap for the tiles is vertical!
  //    The first byte are the pixels on the left.
  //    The lowest bit is the pixel on top.
  PROGMEM const uint8_t batteryTile[] = 
  { 
    // The upper-left 8x8 pixels:
    0x00, 0x00, 0xF8, 0x0C, 0x04, 0x02, 0x02, 0x02, 
    // The upper-right 8x8 pixels:
    0x02, 0x02, 0x02, 0x04, 0x0C, 0xF8, 0x00, 0x00, 
    // The lower-left 8x8 pixels:
    0x00, 0x00, 0x7F, 0x40, 0x40, 0x40, 0x40, 0x40, 
    // The lower-right 8x8 pixels:
    0x40, 0x40, 0x40, 0x40, 0x40, 0x7F, 0x00, 0x00, 
  };

  const uint8_t && battery_percent = map( analogRead(PIN::Energy::BATTERY) , 0 , 1024 , 0 , 100 );

  uint8_t tiles[sizeof(batteryTile)];

  memcpy(tiles, batteryTile, 32);
  PROGMEM constexpr uint8_t  batX = 12;     // the 'x' (column) of the upper-left of the battery
  PROGMEM constexpr uint8_t batY = 3;     // the 'y' (row) of the upper-left of the battery

  // There are 12 lines to draw inside the battery.
  // Zero lines is also an option, so there are 13 possible levels.
  // There are 6 lines in the lower part and 6 lines in the upper part
  uint8_t lines = battery_percent / 8U; // might need some tuning near 0% and 100%
  lines = constrain( lines, 0, 12);     // for safety
  uint8_t lowerlines = min( (const uint8_t) uint8_t(6U), lines);      // 0...6 lines in the lower part
  uint8_t upperlines = 0U;
  if( lines > 6U)
  upperlines = lines - 6U;

  // The lines are over 4 bytes, and the tiles are vertical.
  // I can not think of any good code, so I just put down what it should do.

  // lower-left
  for( uint8_t i=3U; i<8U; i++)
  {
    for( uint8_t j=0U; j<lowerlines; j++)
      bitSet( tiles[i+16U], 7-(j+2U));
  }

  // lower-right
  for( uint8_t i=0U; i<5U; i++)
  {
    for( uint8_t j=0; j<lowerlines; j++)
      bitSet( tiles[i+24U], 7U-(j+2U));
  }

  // upper-left
  for( uint8_t i=3U; i<8U; i++)
  {
    for( uint8_t j=0; j<upperlines; j++)
      bitSet( tiles[i], 7U-j);
  }

  // upper-right
  for( uint8_t i=0U; i<5U; i++)
  {
    for( uint8_t j=0; j<upperlines; j++)
      bitSet( tiles[i+8U], 7U-j);
  }

  //display.drawTile( batX, batY,   2, tiles);                      // 50%-100%
  display.drawBitmap(batX, batY, tiles, 0, 8, SH110X_WHITE);        // 50%-100%
  //display.drawTile( batX, batY+1, 2, (uint8_t *) tiles + 16U);    // 50%-0%
  display.drawBitmap(batX, batY+1, tiles+16U, 0, 8, SH110X_WHITE);  // 50%-0%
  // Bloquear el semáforo
  xSemaphoreTake( semaphoreDisplay , portMAX_DELAY );

  // Desbloquear el semáforo
  xSemaphoreGive( semaphoreDisplay );
*/