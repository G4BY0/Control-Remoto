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
#include <Wifi.h>
#include <SPI.h> 
#include <ESP32Time.h>          //Built-IN RTC
#include "Tasks.h"
#include "Modes.h"

SPIClass spi; // Medio de Comunicacion con el Almacenamiento

//Frecuencia de Actualizacion del Clock (En Milisegundos)
#define REFRESH_CLOCK 500 
// Retocar macro en caso de querer establecer cuanto se pone el modo apagado luego de estar ese tiempo en el modo SLEEPING (En Segundos)
#define SLEEP_TIME_WAITING_TO_SHUTDOWN (5U * 60U)   // 5 minutos en segundos

// Retocar macro en caso de querer establecer cuanto tiempo el usuario debe mantener el boton back para poner el modo SLEEPING (En Segundos)
#define SLEEP_TIME_BUTTONPRESSING (5U)

#define CLOCK_ON
#define WIFI_ON  
//#define BLUETOOTH_ON

#define SSID_IN         "TP-LINK_4F48"        //SSID del access point en el que se conectara el servicio WiFi
#define PASSWORD_IN     "51807511"            //PASSWORD del access point en el que se conectara el servicio WiFi

#define SSID_OUT        "REMOTE_CONTROLLER"   //SSID del access point que se generara
#define PASSWORD_OUT    "SARAGOYLAJEFA"       //PASSWORD del access point que se generara

using namespace MODE;

ESP32Time clock;

void setup(){
    
    Serial.begin(115200);
    while (!Serial); // wait for serial port to connect. Needed for native USB port only
    
    // Voy a usar los puertos de VSPI para la comunicacion SPI (Almacenamiento)
    spi=SPIClass(VSPI); 
    spi.begin();

    #ifdef DEBUG
    // Just to know which program is running on my Sketch
    Serial.println(F("START " __FILE__ " from " __DATE__ "."));

    //Aviso del compilador utilizado (usando los identificadores de cada uno)                                        
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
    #endif                                                                                        
    

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

    #ifdef WIFI_ON
    //  Inicializacion del servicio WiFi
    // Conectar a la red WiFi
    WiFi.begin(SSID_IN, PASSWORD_IN);

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Conectando a WiFi...");
    }

    // Seteo del servicio del Clock con NTC
    // Configurar la zona horaria
    clock.setTimeZone("America/Argentina");

    #endif

    // Espero a que todos los procesos terminen para inicializar
    Serial.flush(); yield();

    // Task para correr el programa principal
    xTaskCreate(

        Task_Idle,                  // Funcion codigo del Task
        "Task_Loop",                // Nombre del Task 
        15000U,                     // Reserva de espacio en la Pila
        NULL,                       // Argumentos
        tskIDLE_PRIORITY,           // Prioridad
        &handleIdle                 // Handle 

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

        Task_WatchDogTimer,         // Funcion codigo del Task
        "Task_WatchDogTimer",       // Nombre del Task
        1500U,                      // Reserva de espacio en la Pila
        NULL,                       // Argumentos
        tskIDLE_PRIORITY + 2U,      // Prioridad
        NULL                        // Sin Handle
    
    );
    
    #ifdef CLOCK_ON
    // Crear tarea del Reloj
    xTaskCreate(

        Task_Clock,                 // Funcion codigo del Task
        "Task_Clock",               // Nombre del Task 
        configMINIMAL_STACK_SIZE,   // Reserva de espacio en la Pila
        NULL,                       // Argumentos
        tskIDLE_PRIORITY,           // Prioridad
        &handleClock                // Handle 
    
    );
    #endif

    #ifdef WIFI_ON
    // Crear tarea para el funcionamiento WI-FI
    xTaskCreate(

        Task_Wifi,                  // Funcion codigo del Task
        "Task_Wifi",                // Nombre del Task 
        configMINIMAL_STACK_SIZE,   // Reserva de espacio en la Pila
        NULL,                       // Argumentos
        tskIDLE_PRIORITY,           // Prioridad
        &handleWiFi                 // Handle 
    
    );
    #endif

    #ifdef BLUETOOTH_ON
    // Crear tarea para el funcionamiento BLUETOOTH
    xTaskCreate(

        Task_Bluetooth,             // Funcion codigo del Task
        "Task_Bluetooth",           // Nombre del Task 
        configMINIMAL_STACK_SIZE,   // Reserva de espacio en la Pila
        NULL,                       // Argumentos
        tskIDLE_PRIORITY,           // Prioridad
        &handleBluetooth            // Sin Handle 
    
    );
    #endif

    // Iniciar el scheduler de FreeRTOS
    //vTaskStartScheduler(); // Genera un core dump si se permite el uso del void loop() Asegurarse de usar si es que no se usa el void loop() (Generado por el Watch Dog Timer)

}


//Solamente se encarga de hacer el reinicio si es pedido
void loop(){  } 