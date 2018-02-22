//TODO: write code to easily program IR signal to code (later do this with android app)
//CHQ1838: Black date facing towards you, left to right
//OUT, GND, VCC (works with both 5V and 3.3V)

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
    long dataHEX = (results.value, HEX);//received code in HEX form
    long targetHEX = irStored.irEEPROMCode;//stored key IR code already in HEX form
    if (DEBUG) {
      Serial.print("Received IR Code in HEX: ");
      Serial.println(dataHEX);
    }
    if (dataHEX == targetHEX) {//if codes match
      if (DEBUG) {
        Serial.println("IR code received matches stored code!");
      }
      reverseRelay();//toggles relay
    }
    else {
      if (DEBUG) {
        Serial.print("IR code received does NOT match stored code. Stored code: ");
        Serial.println(targetHEX);
      }
    }
    irrecv.resume();// Receive the next value
  }
}

//for debugging
void printIRCode() {
  if (irrecv.decode(&results)) {//if an IR code is received
    Serial.print("Received decimal value: ");
    Serial.print(results.value, DEC); // Print the Serial 'results.value'
    Serial.print(". Received hexadecimal value: ");
    Serial.println(results.value, HEX);
    irrecv.resume();   // Receive the next value
  }
}

//https://github.com/svetli/Arduino-IR-Control try looking at this

void storeIRCode() {
  if (irrecv.decode(&results)) {//if an IR code is received
    long receivedCodeDEC = (results.value);//decimal form
    long receivedCodeHEX = (results.value, HEX);//hexadecimal form
    if (DEBUG) {
      Serial.println(receivedCodeHEX); // Print the Serial 'results.value'
    }
    //    clearEEPROM();
    if (saveIR) {//if mode to save IR code as key code is enabled
      if (receivedCodeDEC != -1) {//when it is not FFFFFFFF. This is the value when a button is held down
        irStored.irEEPROMCode = (receivedCodeHEX);//stores HEX value of code into irStored Struct
        EEPROM_writeAnything (0, irStored);//saves configuration in EEPROM for irStored Struct
      }
      irrecv.resume();   // Receive the next value
    }
  }
}

void btPrintIR() {
  String msg = String(irStored.irEEPROMCode);//sends saved IR code.
  BT.println(msg);
}

