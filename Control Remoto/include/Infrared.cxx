#include "Infrared.h"

#define DELAY_BETWEEN_REPEAT 50
#define DISABLE_LEDFEEDBACK 0x0 // false
#define ENABLE_LEDFEEDBACK 0x1  // true

void infraredBegin(void){

  pinMode(PIN::InfraredReceiver::DATA, INPUT);
  pinMode(PIN::InfraredTransmitter::DATA, OUTPUT);

  // Just to know which program is running on my Arduino
  Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRREMOTE));

  // Start the receiver and if not 3. parameter specified, take LED_BUILTIN pin from the internal boards definition as default feedback LED
  IrReceiver.begin(PIN::InfraredReceiver::DATA, DISABLE_LED_FEEDBACK, false);
  Serial.print(F("Ready to receive IR signals of protocols: "));
  printActiveIRProtocols(&Serial);
  
  Serial.println(F("at pin 11")); // PIN USADO PARA EL RECEIVER     (//Serial.print(F("at pin " STR(IR_RECEIVE_PIN)));)

  //IrSender.begin(PIN::InfraredTransmitter::DATA); // Start with IR_SEND_PIN as send pin and enable feedback LED at default feedback LED pin
  IrSender.begin(PIN::InfraredTransmitter::DATA, DISABLE_LED_FEEDBACK, false); // si no funciona, chequear la de arriba
}

void Receive_start(void){

  // Restart receiver too
  Serial.println(F("Start Receiving for infrared signals"));
  IrReceiver.start();

}

void Receive_check(void){

  if (IrReceiver.decodedIRData.rawDataPtr->rawlen < 4) {
    Serial.print(F("Ignore data with rawlen="));
    Serial.println(IrReceiver.decodedIRData.rawDataPtr->rawlen);
    return;
   }
  if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT) {
    Serial.println(F("Ignore repeat"));
    return;
  }
  if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_AUTO_REPEAT) {
    Serial.println(F("Ignore autorepeat"));
    return;
  }
  if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_PARITY_FAILED) {
    Serial.println(F("Ignore parity error"));
    return;
  }
  if (IrReceiver.decode()) {
    //Store received data and resume
    storeCode();
    IrReceiver.resume(); //Resume receiver
  }
}

void Receive_stop(void){
  
  Serial.println(F("Stop receiving"));
  IrReceiver.stop();

}

void sendCode(storedIRDataStruct *aIRDataToSend) {

  if (aIRDataToSend->receivedIRData.protocol == UNKNOWN /* i.e. raw */) {
    // Assume 38 KHz
    IrSender.sendRaw(aIRDataToSend->rawCode, aIRDataToSend->rawCodeLength, 38);

    Serial.print(F("raw "));
    Serial.print(aIRDataToSend->rawCodeLength);
    Serial.println(F(" marks or spaces"));
  } else {
    // Use the write function, which does the switch for different protocols
    IrSender.write(&aIRDataToSend->receivedIRData);
    printIRResultShort(&Serial, &aIRDataToSend->receivedIRData, false);
  }
}

// Stores the code for later playback
void storeCode(void) {
  
  

  storedIRDataStruct* sStoredIRData = new storedIRDataStruct;

  //Copy decoded data
  sStoredIRData->receivedIRData = IrReceiver.decodedIRData;

  if (sStoredIRData->receivedIRData.protocol == UNKNOWN) {
    Serial.print(F("Received unknown code and store "));
    Serial.print(IrReceiver.decodedIRData.rawDataPtr->rawlen - 1);
    Serial.println(F(" timing entries as raw "));
    IrReceiver.printIRResultRawFormatted(&Serial, true); // Output the results in RAW format
    sStoredIRData->rawCodeLength = IrReceiver.decodedIRData.rawDataPtr->rawlen - 1;
    
    //Store the current raw data in a dedicated array for later usage
    IrReceiver.compensateAndStoreIRResultInArray(sStoredIRData->rawCode);
  } else {
    IrReceiver.printIRResultShort(&Serial);
    IrReceiver.printIRSendUsage(&Serial);
    sStoredIRData->receivedIRData.flags = 0; // clear flags -esp. repeat- for later sending
    Serial.println();
  }

  delete[] sStoredIRData;
  
}