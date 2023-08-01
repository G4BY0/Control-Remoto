//Copyright Grupo 11, Inc. All Rights Reserved.
/***********************************************
 * * * * * * * * * * * * * * * * * * * * * * * *
 * 
 * Source code of members of Infrared.h
 * 
 * * * * * * * * * * * * * * * * * * * * * * * *
***********************************************/

#include "Infrared.h"

// The IR transmitter.
IRrecv irrecv(kRecvPin, kCaptureBufferSize, kTimeout, false);
// The IR receiver.
IRsend irsend(kIrLedPin);
// Somewhere to store the captured message.
decode_results results;

void infraredBegin(void){

  pinMode(PIN::InfraredReceiver::DATA, INPUT);
  pinMode(PIN::InfraredTransmitter::DATA, OUTPUT);
  
  // El IRreceiver inicializa en su constructor

  // Inicializacion del IRSender
  irsend.begin(); // Start up the IR sender.

}

void Receive_start(void){
  // Inicializacion del IRReceiver
  irrecv.enableIRIn();  // Start up the IR receiver.
  irrecv.resume(); // Resume receiver
}

bool Receive_check(void){

  // Logica de checkeo si la informacion recibida es correcta o incorrecta...
  return ( irrecv.decode(&results) ? true : false );
  
}

void Receive_stop(void){
  irrecv.disableIRIn(); // Disable any timers and interrupts.
}

void sendCode(std::shared_ptr<storedIRDataStruct> IRData) {
  Serial.flush(); // To avoid disturbing the software PWM generation by serial output interrupts

  bool success = true;
  uint16_t size = IRData->results.bits;
  decode_type_t protocol = IRData->results.decode_type;
  Serial.flush();
  if (protocol == decode_type_t::UNKNOWN) {  // Yes.
      // Convert the results into an array suitable for sendRaw().
      // resultToRawArray() allocates the memory we need for the array.
      uint16_t *raw_array = resultToRawArray(&IRData->results);
      // Find out how many elements are in the array.
      size = getCorrectedRawLength(&IRData->results);

      // Send it out via the IR LED circuit.
      irsend.sendRaw(raw_array, size, kFrequency);

      // Deallocate the memory allocated by resultToRawArray().
      delete [] raw_array;
    } else if (hasACState(protocol)) {  // Does the message require a state[]?
      // It does, so send with bytes instead.
      success = irsend.send(protocol, IRData->results.state, size / 8);
    } else {  // Anything else must be a simple message protocol. ie. <= 64 bits
      success = irsend.send(protocol, IRData->results.value, size);
    }
    if(success == false) 
     Serial.println(F("Failed sending Signal!"));
    else {
    // Display a crude timestamp & notification.
    uint32_t now = millis();
    Serial.printf(
        "A %d-bit %s message was %ssuccessfully retransmitted.\n"
        , size, typeToString(protocol).c_str(),
        success ? "" : "un");
    }
}

// Stores the code for later playback
std::shared_ptr<storedIRDataStruct> storeCode( const char* subprofileName  ) {

  std::shared_ptr<storedIRDataStruct> IRData(new storedIRDataStruct);

  // Copy decoded data
  std::copy( &results , &results + 1 , &IRData->results );
  // Copy Sub-Profile String
  for(uint8_t iterator = 0; subprofileName[iterator] != '\0'; iterator++) IRData->nameSubProfile[iterator] = subprofileName[iterator]; 
  
  return IRData;

}