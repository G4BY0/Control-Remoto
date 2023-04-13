#include "Infrared.h"

#define DELAY_BETWEEN_REPEAT 50
#define SEND_BUTTON_PIN PIN::Buttons::ENTER

bool sSendButtonWasActive;

void infraredBegin(void){

  // Just to know which program is running on my Arduino
  Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRREMOTE));

  // Start the receiver and if not 3. parameter specified, take LED_BUILTIN pin from the internal boards definition as default feedback LED
  IrReceiver.begin(PIN::InfraredReceiver::DATA, ENABLE_LED_FEEDBACK);
  Serial.print(F("Ready to receive IR signals of protocols: "));
  printActiveIRProtocols(&Serial);
  Serial.println(F("at pin " STR(IR_RECEIVE_PIN)));

  IrSender.begin(PIN::InfraredTransmitter::DATA); // Start with IR_SEND_PIN as send pin and enable feedback LED at default feedback LED pin



}

void Receive(storedIRDataStruct *sStoredIRData){

  // If button pressed, send the code.
  bool tSendButtonIsActive = (digitalRead(SEND_BUTTON_PIN) == LOW); // Button pin is active LOW

  /*
   * Check for current button state
  */
  if (tSendButtonIsActive) {
    if (!sSendButtonWasActive) {
      Serial.println(F("Stop receiving"));
      IrReceiver.stop();
    }
    /*
    * Button pressed -> send stored data
    */
    //Serial.print(F("Button pressed, now sending "));
    //if (sSendButtonWasActive == tSendButtonIsActive) {
    //  Serial.print(F("repeat "));
    //  sStoredIRData->receivedIRData.flags = IRDATA_FLAGS_IS_REPEAT;
    //} else {
    //  sStoredIRData->receivedIRData.flags = IRDATA_FLAGS_EMPTY;
    //}
    //Serial.flush(); // To avoid disturbing the software PWM generation by serial output interrupts
    //sendCode(sStoredIRData);
    //delay(DELAY_BETWEEN_REPEAT); // Wait a bit between retransmissions
//
    //} else if (sSendButtonWasActive) {
    //  /*
    //  * Button is just released -> activate receiving
   //  */
    //  // Restart receiver
    //  Serial.println(F("Button released -> start receiving"));
    //  IrReceiver.start();

   // } else if (IrReceiver.decode()) {
      /*
      * Button is not pressed and data available -> store received data and resume
      */
    ///  storeCode();
    //  IrReceiver.resume(); // resume receiver
    //}

    sSendButtonWasActive = tSendButtonIsActive;
    delay(100);
    
}

    


void sendCode(storedIRDataStruct *aIRDataToSend) {
    if (aIRDataToSend->receivedIRData.protocol == UNKNOWN /* i.e. raw */) {
        // Assume 38 KHz
        IrSender.sendRaw(aIRDataToSend->rawCode, aIRDataToSend->rawCodeLength, 38);

        Serial.print(F("raw "));
        Serial.print(aIRDataToSend->rawCodeLength);
        Serial.println(F(" marks or spaces"));
    } else {

        /*
         * Use the write function, which does the switch for different protocols
         */
        IrSender.write(&aIRDataToSend->receivedIRData);
        printIRResultShort(&Serial, &aIRDataToSend->receivedIRData, false);
    }
}



// Stores the code for later playback in sStoredIRData
// Most of this code is just logging
void storeCode(storedIRDataStruct* sStoredIRData) {
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
  /*
  * Copy decoded data
  */
  sStoredIRData->receivedIRData = IrReceiver.decodedIRData;

  if (sStoredIRData->receivedIRData.protocol == UNKNOWN) {
    Serial.print(F("Received unknown code and store "));
    Serial.print(IrReceiver.decodedIRData.rawDataPtr->rawlen - 1);
    Serial.println(F(" timing entries as raw "));
    IrReceiver.printIRResultRawFormatted(&Serial, true); // Output the results in RAW format
    sStoredIRData->rawCodeLength = IrReceiver.decodedIRData.rawDataPtr->rawlen - 1;
    /*
     * Store the current raw data in a dedicated array for later usage
     */
    IrReceiver.compensateAndStoreIRResultInArray(sStoredIRData->rawCode);
  } else {
    IrReceiver.printIRResultShort(&Serial);
    IrReceiver.printIRSendUsage(&Serial);
    sStoredIRData->receivedIRData.flags = 0; // clear flags -esp. repeat- for later sending
    Serial.println();
  }

}