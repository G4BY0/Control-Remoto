//Copyright Grupo 11, Inc. All Rights Reserved.
/***********************************************
 * * * * * * * * * * * * * * * * * * * * * * * *
 * 
 * Source code of Tasks.h
 * 
 * * * * * * * * * * * * * * * * * * * * * * * *
***********************************************/

#include "Tasks.h"

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

void Task_Sleep(void* __nonParameter){ // Como la funcion no recibe parametros no lo voy a usar el argumento
    while(1){

        for(unsigned long countPressed = millis(); buttonState(PIN::Buttons::BACK) == true; )
        if ( ( millis() - countPressed ) >= SLEEP_TIME_BUTTONPRESSING ){
            
            // Control de modo Sleeping Hasta el tiempo estipulado, si pasa entrara en ShutDown
            MODE::sleep(SLEEP_TIME_WAITING_TO_SHUTDOWN);

            // Pausar la tarea durante un breve periodo de tiempo
            vTaskDelay(pdMS_TO_TICKS(1000U)); // Pausa de 1000 milisegundos (1 segundo)
        }
        
        // Pausar la tarea durante un breve periodo de tiempo
        vTaskDelay(pdMS_TO_TICKS(1000U)); // Pausa de 1000 milisegundos (1 segundo)
        
    }
    #undef SLEEP_TIME_BUTTON_PRESSING
    #undef SLEEP_TIME_WAITING_TO_SHUTDOWN  
    
}

void Task_Restart(void* __nonParameter){
    
    // Verificar si se debe reiniciar la placa
    if (MODE::ShutDown::shouldRestart) { //Antes de reiniciar la placa deben haber terminado o suspendido los Tasks

        //disconnect WiFi as it's no longer needed
        WiFi.disconnect(true);
        WiFi.mode(WIFI_OFF);
        //Elimino todos los Tasks del sistema
        vTaskDelete(handleIdle);
        vTaskDelete(handleBattery);
        #ifdef CLOCK_ON
        vTaskDelete(handleClock);
        #endif
        #ifdef WIFI_ON
        vTaskDelete(handleWiFi);
        #endif
        #ifdef BLUETOOTH_ON
        vTaskDelete(handleBluetooth);
        #endif
        // Le doy tiempo a los demas Tasks de liberar los recursos que estaban consumiendo
        vTaskDelay(pdMS_TO_TICKS(1000U)); // Pausa de 1000 milisegundos (1 segundo)
        //Reinicio del Sistema, se despierta del ShutDown
        #if defined(ESP32) || defined(ESP8266)
        ESP.restart();
        #else
        asm volatile("reset"); 
        #endif
      
    }

    vTaskDelete(NULL);

}

void Task_Idle(void * __nonParameter){ 
    
    while(1){
        #ifdef DEBUG
        Serial.printf("%s Pila Consume: %d" ,pcTaskGetName ,uxTaskGetStackHighWaterMark(NULL) );
        #endif
        MODE::hub();
    } 
}

void Task_WatchDogTimer(void* parameter) {  
    TickType_t lastWakeTime = xTaskGetTickCount();

    while (true) {
        // Realimentación del WDT
        // Puedes utilizar xTaskNotify() o cualquier otro mecanismo para enviar una notificación a la tarea de supervisión

        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(WATCHDOG_FEED_PERIOD_MS));
  }
}

ESP32Time RTC;
void Task_Clock(void* __nonParameter){

    while(1){
        struct tm time;
        #ifdef WIFI_ON
        if(!getLocalTime(&time)){
            Serial.println(F("Failed to obtain time."));
            return;
        }
        #else
        time.tm_year = RTC.getHour(true);    
        time.tm_mon  = RTC.getMonth();
        time.tm_mday = RTC.getDay();     
        time.tm_hour = RTC.getHour();
        time.tm_min  = RTC.getMinute();
        time.tm_sec  = RTC.getSecond();
        #endif
        Interface::clock(time);
        
        // Pausar la tarea durante un breve periodo de tiempo
        vTaskDelay(pdMS_TO_TICKS(REFRESH_CLOCK)); // Pausa de refresh
    }


}


bool __Wifi = false;
void Task_Wifi(void* __nonParameter){

  while(1){
  

  
  // Pausar la tarea durante un breve periodo de tiempo
  vTaskDelay(pdMS_TO_TICKS(1000)); // Pausa de 1000 milisegundos (1 segundo)
  }


}

void Task_Bluetooth(void* __nonParameter){}
