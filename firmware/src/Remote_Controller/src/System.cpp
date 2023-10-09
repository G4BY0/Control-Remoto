#include "System.h"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

bool __wifi = false;
bool __bluetooth = false;

void Task_Idle(void * __nonParameter){ 

  vTaskDelete(NULL);
}

void Task_WatchDogTimer(void* parameter) {  
    TickType_t lastWakeTime = xTaskGetTickCount();

    // Realimentación del WDT
    // Puedes utilizar xTaskNotify() o cualquier otro mecanismo para enviar una notificación a la tarea de supervisión
    while (true) vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(WATCHDOG_FEED_PERIOD_MS));
    
}
#undef WATCHDOG_FEED_PERIOD_MS

void __sleep(uint32_t Seconds){
  auto countSleep = millis();
  //Logica de si se llegara a pulsar cualquier boton
  while(!(  buttonState(PIN::Buttons::BACK ) == HIGH  ||
            buttonState(PIN::Buttons::UP   ) == HIGH  ||
            buttonState(PIN::Buttons::DOWN ) == HIGH  ||
            buttonState(PIN::Buttons::LEFT ) == HIGH  ||
            buttonState(PIN::Buttons::RIGHT) == HIGH  || 
            buttonState(PIN::Buttons::ENTER) == HIGH     ))
    if( ( millis() - countSleep ) >= Seconds ) 
      ShutDown_now();
}

void Task_AFK(void* __nonParameter){ // Como la funcion no recibe parametros no lo voy a usar el argumento
    while(1){
      for(auto countPressed = millis(); buttonState(PIN::Buttons::BACK) == true; )
      if ( ( millis() - countPressed ) >= SLEEP_TIME_BUTTONPRESSING ){   
        // Control de modo Sleeping Hasta el tiempo estipulado, si pasa entrara en ShutDown
        __sleep(SLEEP_TIME_WAITING_TO_SHUTDOWN);
        // Pausar la tarea durante un breve periodo de tiempo
        vTaskDelay(pdMS_TO_TICKS(1000U)); // Pausa de 1000 milisegundos (1 segundo)
      }
      // Pausar la tarea durante un breve periodo de tiempo
      vTaskDelay(pdMS_TO_TICKS(1000U)); // Pausa de 1000 milisegundos (1 segundo)
    }

}
#undef SLEEP_TIME_WAITING_TO_SHUTDOWN
#undef SLEEP_TIME_BUTTONPRESSING       
#undef SLEEP_TIME_AFK

void restart_now(void){
  //Reinicio del Sistema
  ESP.restart();
  
}

void ShutDown_now(void) { 

  ShutDown::displayService(); //Servicio de pantalla off
  
  ShutDown::SDService(); //Servicio de Almacenamiento off
  
  SPI.end(); //Servicio de comunicacion SPI off


  Serial.flush(); // Asegura que se envíen los datos pendientes antes de apagar el Serial
  
  Serial.end(); //Servicio de comunicacion Serial (Rx/Tx) off

  ShutDown::buttonsWaiting(); //Servicio de botonera en espera
  restart_now();

}

void ShutDown::displayService(void){ display.clearDisplay(); display.display(); }

void ShutDown::SDService(void)     { SD.end(); }

void ShutDown::buttonsWaiting(void){

  //Dando espera a la pulsacion de algun boton
  while(!(  buttonState(PIN::Buttons::BACK ) == HIGH  ||
            buttonState(PIN::Buttons::UP   ) == HIGH  ||
            buttonState(PIN::Buttons::DOWN ) == HIGH  ||
            buttonState(PIN::Buttons::LEFT ) == HIGH  ||
            buttonState(PIN::Buttons::RIGHT) == HIGH  || 
            buttonState(PIN::Buttons::ENTER) == HIGH     ));

}

TaskHandle_t handleBattery;     //Handle al Task de mostrar la bateria
TaskHandle_t handleSleep;       //Handle al Task de SLEEPING
TaskHandle_t handleIdle;        //Hanlde al Task del idle
TaskHandle_t handleClock;       //Handle al Task de Clock
TaskHandle_t handleWiFi;        //Handle al Servicio Wifi
TaskHandle_t handleBluetooth;   //Handle al Servicio Bluetooth


void Task_Battery(void* __nonParameter){  // Como la funcion no recibe parametros no lo voy a usar el argumento     
    while(1){
        Interface::battery();
        // Pausar la tarea durante un breve periodo de tiempo
        vTaskDelay(pdMS_TO_TICKS(1000U)); // Pausa de 1000 milisegundos (1 segundo)
    }
}

#define WIFI_ON
ESP32Time RTC;
bool __clock = false;
void clockBegin(void){
    struct tm time;
    //Si el Wifi Esta Habilidado, la sincronizo con Internet
    #ifdef WIFI_ON
    timeClient.begin();
    timeClient.update();
    time.tm_mday = timeClient.getDay();
    time.tm_hour = timeClient.getHours();
    time.tm_hour = timeClient.getMinutes();
    time.tm_sec = timeClient.getSeconds();
    RTC.setTimeStruct(time);
    #else
    if(!getLocalTime(&time)){
        Serial.println(F("Failed to obtain time."));
    }
    RTC.setTimeStruct(time);
    #endif

}
