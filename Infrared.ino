//TODO: write code to easily program IR signal to code (later do this with android app)

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

}

void printIRCode() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value, DEC); // Print the Serial 'results.value'
    irrecv.resume();   // Receive the next value
  }
}

//https://github.com/svetli/Arduino-IR-Control try looking at this

void storeIRCode() {//use EEPROM
  
//  if (irrecv.decode(&results)) {
//    Serial.println(results.value, DEC); // Print the Serial 'results.value'
//    EEPROM_writeAnything (0, (results.value, DEC));//stores the received data in decimal in spot 0 in EEPROM using EEPROMAnything.h or actually moves to new spot and stores it, need to check EEPROMAnything.h
//    irrecv.resume();   // Receive the next value
//  }
}

void btPrintIR() {
  String msg = "Xplaceholder";
  BT.println(msg);
}

