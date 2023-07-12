//Copyright Grupo 11, Inc. All Rights Reserved.
/***********************************************
 * * * * * * * * * * * * * * * * * * * * * * * *
 * \file
 * Main.cpp (Main File)
 * 
 * * * * * * * * * * * * * * * * * * * * * * * *
***********************************************/
#warning "Project is in developing, it's not already yet!"

#include <Arduino.h>
#include <Wire.h>               // I2C
#include <SPI.h> 
#include <freertos/FreeRTOS.h>  //FreeRTOS Operative System
#include <freeRTOS/task.h>      //MultiThreading
#include <freeRTOS/semphr.h>    //Semaphore
#include <ESP32Time.h>          //Built-IN RTC
#include "Modes.h"

//Frecuencia de Actualizacion del Clock
#define REFRESH_CLOCK 500 //Milisegundos
// Retocar macro en caso de querer establecer cuanto se pone el modo apagado luego de estar ese tiempo en el modo SLEEPING
#define SLEEP_TIME_WAITING_TO_SHUTDOWN (5U * 60U) // 5 minutos en segundos
// Retocar macro en caso de querer establecer cuanto tiempo el usuario debe mantener el boton back para poner el modo SLEEPING
#define SLEEP_TIME_BUTTONPRESSING (5U) // 5 segundos


  
SPIClass spi;

TaskHandle_t handleBattery; //Handle al Task de mostrar la bateria
TaskHandle_t handleSleep; //Handle al Task de SLEEPING
TaskHandle_t handleLoop; //Task de mi propio "loop", pero no pertenece al Idle (programa Principal)


using namespace MODE;


/*! \brief Task para mostrar de manera dinamica la bateria
    \note lo muestra en esquinas de la pantalla */
void Task_Battery(void* nonParameters){  // Como la funcion no recibe parametros no lo voy a usar el argumento     
    while(1){
        Interface::battery();
        // Pausar la tarea durante un breve periodo de tiempo
        vTaskDelay(pdMS_TO_TICKS(1000U)); // Pausa de 1000 milisegundos (1 segundo)

    }
}

/*! \brief Task para preguntar de manera dinamica si se muestran sintomas de dormir
    \note El boton BACK se debe mantener presionado el tiempo determinado -> 'SLEEP_TIME_BUTTONPRESSING' */
void Task_Sleep(void* nonParameters){ // Como la funcion no recibe parametros no lo voy a usar el argumento
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

//Parte todo del Hub y luego se ramifica en los demas Menus
void Task_Loop(void * nonParameters){ while(1)hub(); }

#define WATCHDOG_FEED_PERIOD_MS (1000U)  // Realimentación del WDT cada 1 segundo
void Task_WatchDogTimer(void* parameter) {  
    TickType_t lastWakeTime = xTaskGetTickCount();

    while (true) {
        // Realimentación del WDT
        // Puedes utilizar xTaskNotify() o cualquier otro mecanismo para enviar una notificación a la tarea de supervisión

        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(WATCHDOG_FEED_PERIOD_MS));
  }
}

/*
void Task_Clock(void* nonParameter){

  while(1){
  
  
  // Pausar la tarea durante un breve periodo de tiempo
  vTaskDelay(pdMS_TO_TICKS(REFRESH_CLOCK)); // Pausa de refresh
  }


}
*/
/*
bool Wifi_off = false;
bool Wifi_ = false;
void Task_Wifi(void* nonParameter){

  while(1){
  
  
  // Pausar la tarea durante un breve periodo de tiempo
  vTaskDelay(pdMS_TO_TICKS(1000)); // Pausa de 1000 milisegundos (1 segundo)
  }


}
*/
void setup(){
    
    Serial.begin(115200);
    while (!Serial); // wait for serial port to connect. Needed for native USB port only
    
    // Voy a usar los puertos de VSPI para la comunicacion SPI (Almacenamiento)
    spi=SPIClass(VSPI); 
    spi.begin();

    // Just to know which program is running on my Sketch
    Serial.println(F("START " __FILE__ " from " __DATE__ "."));

    //Aviso del compilador utilizado (usando los identificadores de cada uno)
    #define UsedCompiler  \                                       
    Serial.println(F("Tipo de compilador Utilizado: "));                                                \
    #if defined(__GNUC__)                                                                               \
        Serial.println(F("GNU :)"));                                                                    \
        Serial.print(F("Version del compilador de GNU es: "));                                          \
        Serial.println(F(__GNUC__));                                                                    \
    #elif defined(__clang__)                                                                            \
        Serial.println(F("CLANG !!"));                                                                  \
        Serial.println(F("Version Principal del compilador de CLANG es: "  __clang_major__ ));          \
        Serial.println(F("Version Secundaria del compilador de CLANG es: " __clang_minor__ ));          \
        Serial.println(F("Nivel de parche del compilador de CLANG: "       __clang_patchlevel__ ));     \
    #else                                                                                               \
        Serial.println(F("Generico"));                                                                  \
    #endif                                                                                              
    
    
    //Descomentar para ver: Aviso del compilador utilizado (usando los identificadores de cada uno)
    //UsedCompiler;

    #undef UsedCompiler

    //Descomentar en Caso de querer saber cual fue el compilador utilizado
    //UsedCompiler;

    // Inicializacion del sistema del display
    displayBegin();     Serial.println(F("Display Inicializado"));

    // Inicializacion del sistema de botones
    buttonsBegin();     Serial.println(F("Botonera Inicializada"));

    // Inicializacion del sistema de almacenamiento
    SDBegin();          Serial.println(F("Almacenamiento Inicializado"));

    // Inicializacion del sistema del infrarrojo
    infraredBegin();    Serial.println(F("Infrared Inicializado"));

    // Espero a que todos los procesos terminen para inicializar
    Serial.flush(); yield();

    // Task para mostrar la bateria en el display de forma dinamica
    xTaskCreate(

        Task_Loop,                  // Funcion codigo del Task
        "Task_Loop",                // Nombre del Task 
        15000U,                     // Reserva de espacio en la Pila
        NULL,                       // Argumentos
        tskIDLE_PRIORITY,           // Prioridad
        &handleLoop                 // Handle 

    );

    // Task para preguntar si el usuario esta activo de forma dinamica 
    xTaskCreate(

        Task_Sleep,                 //Funcion codigo del Task
        "Task_Sleep",               //Nombre del Task 
        1500U,                      //Reserva de espacio en la Pila 
        NULL,                       //Argumentos
        tskIDLE_PRIORITY + 1U,      //Prioridad
        &handleSleep                //Handle   

    );

    // Task para mostrar la bateria en el display de forma dinamica
    xTaskCreate(

        Task_Battery,               // Funcion codigo del Task
        "Task_Battery",             // Nombre del Task
        1500U,                      // Reserva de espacio en la Pila
        NULL,                       // Argumentos
        tskIDLE_PRIORITY,           // Prioridad
        &handleBattery              // Handle

    );

    // Crear tarea del Watchdog Timer
    xTaskCreate(

        Task_WatchDogTimer,               // Funcion codigo del Task
        "Task_WatchDogTimer",       // Nombre del Task 
        1500U,                      // Reserva de espacio en la Pila
        NULL,                       // Argumentos
        tskIDLE_PRIORITY + 2U,      // Prioridad
        NULL                        // Sin Handle 
    
    );
    
    /*
    // Crear tarea del Watchdog Timer
    xTaskCreate(

        Task_Clock,                 // Funcion codigo del Task
        "Task_Clock",               // Nombre del Task 
        configMINIMAL_STACK_SIZE,   // Reserva de espacio en la Pila
        NULL,                       // Argumentos
        tskIDLE_PRIORITY,      // Prioridad
        NULL                        // Sin Handle 
    
    );
    */
    // Iniciar el scheduler de FreeRTOS
    //vTaskStartScheduler(); // Genera un core dump si se permite el uso del void loop() Asegurarse de usar si es que no se usa el void loop() (Generado por el Watch Dog Timer)

}


//Solamente se encarga de hacer el reinicio si es pedido
void loop(){

    
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