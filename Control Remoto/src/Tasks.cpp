#include "Tasks.h"


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
    if (ShutDown::shouldRestart) { //Antes de reiniciar la placa deben haber terminado o suspendido los Tasks
    
        //Elimino todos los Tasks del sistema
        vTaskDelete(handleBattery);
        vTaskDelete(handleLoop);
        // Le doy tiempo a los demas Tasks de liberar los recursos que estaban consumiendo
        vTaskDelay(pdMS_TO_TICKS(1000U)); // Pausa de 1000 milisegundos (1 segundo)
        //Reinicio del Sistema, se despierta del ShutDown
        #if defined(ESP32) || defined(ESP8266)
        ESP.restart();
        #else
        asm volatile("reset"); 
        #endif
      
    }

}


void Task_Loop(void * __nonParameter){ while(1)hub(); }


void Task_WatchDogTimer(void* parameter) {  
    TickType_t lastWakeTime = xTaskGetTickCount();

    while (true) {
        // Realimentación del WDT
        // Puedes utilizar xTaskNotify() o cualquier otro mecanismo para enviar una notificación a la tarea de supervisión

        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(WATCHDOG_FEED_PERIOD_MS));
  }
}


void Task_Clock(void* __nonParameter){

  while(1){
  
  
  // Pausar la tarea durante un breve periodo de tiempo
  vTaskDelay(pdMS_TO_TICKS(REFRESH_CLOCK)); // Pausa de refresh
  }


}


extern bool __Wifi = false;

void Task_Wifi(void* __nonParameter){

  while(1){
  
  
  // Pausar la tarea durante un breve periodo de tiempo
  vTaskDelay(pdMS_TO_TICKS(1000)); // Pausa de 1000 milisegundos (1 segundo)
  }


}
