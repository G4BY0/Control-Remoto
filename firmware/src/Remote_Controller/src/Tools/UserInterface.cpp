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

  sprintf(buffer, "%ld%%" ,map(analogRead(PIN::Energy::BATTERY), 0 , 4095 , 0 , 100 ));
  

}

void UI_t::graphic(void){

  xSemaphoreTake( semaphoreDisplay , portMAX_DELAY ); // Bloquear el semáforo
  display.setTextColor(SH110X_BLACK);
  display.drawRect(0,0,128,8,SH110X_BLACK);

  display.setCursor(0,0);
  display.setTextColor(SH110X_WHITE);
  //Tiempo
  if(show_time == true && WiFi.status() == WL_CONNECTED){
    time_RTC = RTC.getTimeStruct(); // Sincronizo el tiempo a mostrar con el clock del sistema
    char clock_str[13] = {'\0'};
    Interface::clock(time_RTC, clock_str);
    display.print(clock_str);
    display.print(' ');
  }

  //Nivel de bateria en porcentaje
  if(show_battery == true){
    /*(DEV) - FEATURE: SIMBOLS 
    wchar_t* battery_str[5] = {'\0'};*/
    char battery_str[5] = {'\0'};
    batteryPercentage(battery_str);
    display.print(battery_str);
    display.setFont(&icons_UI);
    //display.print();
    display.setFont(NULL);
  }
  if(WIFI_SERVICE_STATUS == true){
    bool wifi_current_status = WiFi.status();
    display.setFont(&icons_UI);
    if(wifi_current_status == WL_CONNECTED){ //When WiFi is conected
      const int8_t wifi_signal = map(WiFi.RSSI(), -127, 0, 1, 5); // dbm Unit
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
  }
  
  //Almacenamiento porcentaje usado
  if(show_storage == true){
    display.setFont(&icons_UI);
    const uint64_t &&storage_max_size = SD.cardSize();
    const uint64_t &&storage_used = SD.usedBytes();
    if(!SD.begin()) {
      display.print(F("N/A"));
      display.setFont(&icons_UI);
      //display.print();
      display.setFont(NULL);
    } else{

      //Hago mi propia funcion map() pero para que retorne uint64_t
      auto map_uint16_t = [](unsigned long long x, unsigned long long in_min, unsigned long long in_max, unsigned long long out_min, unsigned long long out_max) -> uint64_t {
      if (in_max <= in_min || out_max <= out_min) { // Asegurarse de que los límites sean válidos
          return x;
      }
      return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min; // Realizar el mapeo
      };

      display.print(map_uint16_t(SD.cardSize() - SD.usedBytes(), 0ull , SD.cardSize() , 0 , 100));
      display.setFont(&icons_UI);
      //display.print();
      display.setFont(NULL);

    }
    
  }

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

  pinMode(PIN::Energy::BATTERY, INPUT);
  pinMode(PIN::Energy::CHARGE, INPUT);
  pinMode(PIN::Energy::CIRCUIT_MEDITION,OUTPUT);

  digitalWrite(PIN::Energy::CIRCUIT_MEDITION, HIGH); // Close circuit of medition

  while(1){
    if(UI.show == true)
      UI.graphic();
    
    // Pausar la tarea durante un breve periodo de tiempo
    vTaskDelay(pdMS_TO_TICKS(1U));
  }
  vTaskDelete(NULL);
}

void UI_t::stop(void){

  digitalWrite(PIN::Energy::CIRCUIT_MEDITION, LOW); // Open circuit of medition
  vTaskDelete(handle);

}

UI_t::UI_t(void){ }
UI_t::~UI_t(void){ }

UI_t UI;