#include "Modes/WifiNetworks.h"

void MODE::wifiNetworks(void){

  do{

    //Pantalla Emergente que le pregunta al usuario si desea agregar un subperfil en este momento
    //Establezco los parametros a utilizar para la muestra a la salida del display
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);

    display.setCursor(0,0);
    const uint16_t totalNetworks = WiFi.scanNetworks();
    std::vector<String> namesNetworks;
    for (uint16_t i = 0; i < totalNetworks; i++) {
      Serial.println("Nombre: " + WiFi.SSID(i) + ", Potencia de señal: " + String(WiFi.RSSI(i)) + " dBm");
      namesNetworks.push_back(WiFi.SSID(i));
    }

    Cursor cursor(namesNetworks ,display);
    const char*&& selected_network = cursor.getSelectedOption();

  
    WritterV2 writter(&display);
    Serial.print(F("Write The Password of the SSID selected: "));
    const char* password = writter.stringFinished(); 
    Serial.println(password);

    WiFi.begin(selected_network, password);

    if( WiFi.status() == WL_NO_SSID_AVAIL){
      Serial.println(F("La SSID ofrecida, no esta disponible."));
      continue;
    }

    Serial.print(F("Conectando a WiFi... "));
    //Si no se conecta... espera hasta conectarse (senializo con LED_BUILTIN la reconexion)
    for(auto waitingTime = millis() ; WiFi.status() != WL_CONNECTED ; ) {

      if( WiFi.status() == WL_CONNECT_FAILED){
        Serial.println(F("Conexion WiFi Perdida."));
        break;
      }

      if(millis() - waitingTime >= WAITING_TO_CONNECT_MS){
        Serial.println(F("Tiempo de espera expirado"));
        break;
      }

    } 

  } while( WiFi.status() != WL_CONNECTED );

  WIFI_SERVICE_STATUS = true;
  Serial.println(F("Conectado!"));


}

/*
typedef enum {
    WL_NO_SHIELD        = 255,   // for compatibility with WiFi Shield library
    WL_IDLE_STATUS      = 0,
    WL_NO_SSID_AVAIL    = 1,
    WL_SCAN_COMPLETED   = 2,
    WL_CONNECTED        = 3,
    WL_CONNECT_FAILED   = 4,
    WL_CONNECTION_LOST  = 5,
    WL_DISCONNECTED     = 6
} wl_status_t;
*/