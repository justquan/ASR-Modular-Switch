//works 3/4

//CHQ1838: Black dot with X facing towards you, left to right
//OUT, GND, VCC (works with both 5V and 3.3V)
//works

//receives the three char command from the interpretCommand(), and makes sense of it
void infraredInterpret(String filteredData) {
  if (filteredData.equals("SET")) {
    saveIR = true;//the switch is in saveIR mode, so all the IR values it receives are saved ask key values which turn on/off the switch if detected
  }

  else if (filteredData.equals("FIN")) {
    saveIR = false;//the switch is done saving key IR values
  }
}

//TODO: learn how to make system to choose when setting the code and storing to eeprom or clearing eeprom
void infraredSwitch() {
  if (irrecv.decode(&results)) {//if an IR code is received
    EEPROM_readAnything(0, irStored);
    long dataDEC = results.value;//received code in DEC form
    long targetDEC = irStored.irEEPROMCode;//stored key IR code already in DEC form
    if (DEBUG) {
      Serial.print("DEC IR Code:");
      Serial.println(dataDEC);
    }
    if (dataDEC == targetDEC) {//if codes match
      if (DEBUG) {
        Serial.println("IR Match");
      }
      reverseRelay();//toggles relay
      delay(irDebounceInterval);//for debouncing
    }
    else {
      if (DEBUG) {
        Serial.print("No Match. Stored code:");
        Serial.println(targetDEC);
      }
    }
    irrecv.resume();// Receive the next value
  }
}

//for debugging
void printIRCode() {
  if (irrecv.decode(&results)) {//if an IR code is received
    Serial.print("DEC IR:");
    Serial.print(results.value, DEC); // Print the Serial 'results.value'
    Serial.print(". HEX IR:");
    Serial.println(results.value, HEX);
    irrecv.resume();   // Receive the next value
  }
}

//https://github.com/svetli/Arduino-IR-Control try looking at this

void storeIRCode() {
  if (irrecv.decode(&results)) {//if an IR code is received
    long receivedCodeDEC = (results.value);//decimal form
    if (DEBUG) {
      Serial.print("HEX: ");
      Serial.print(receivedCodeDEC, HEX); // Print the Serial 'results.value'
      Serial.print(" DEC: ");
      Serial.println(receivedCodeDEC); // Print the Serial 'results.value'
    }
    if (sendDataToAndroid) {
      btPrintIR(receivedCodeDEC);
    }
    if (saveIR) {//if mode to save IR code as key code is enabled
      if (receivedCodeDEC != -1) {//when it is not FFFFFFFF. This is the value when a button is held down
        irStored.irEEPROMCode = (receivedCodeDEC);//stores HEX value of code into irStored Struct
        EEPROM_writeAnything (0, irStored);//saves configuration in EEPROM for irStored Struct
        Serial.print("IR Target: ");
        Serial.println(irStored.irEEPROMCode);
        btPrintIR(irStored.irEEPROMCode);
      }
    }
    irrecv.resume();   // Receive the next value
  }
}

void btPrintIR(int code) {
  String msg = btSendBlock + String(code);//sends saved key IR code.
  BT.println(msg);
}

