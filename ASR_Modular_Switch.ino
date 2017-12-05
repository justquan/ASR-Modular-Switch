#include "Variables.h" //File with my global variables

void setup() {
  Serial.begin(9600); //baud rate of 9600
  BT.begin(9600);

  //setting pin modes
  pinMode(lightPin, INPUT);// TODO: possibly need to change to just constant pins based on numbers (like pin1 and pin2) instead of pins based on sensors because of the modular design
  pinMode(pirPin, INPUT);
  pinMode(relayPin, OUTPUT);

  irrecv.enableIRIn(); // Start the ir receiver
  // Send test message to other device
  //BT.println("Hello from Arduino");
  timer = 0;

  if(DEBUG){
  pinMode(LED_BUILTIN, OUTPUT);
  }
  EEPROM_readAnything(0, storedCode);//reads whatever is at spot 0 in EEPROM and assigns value to storedCode
  Serial.println(storedCode);//debug
  printAllEEPROMAnything();
}

void loop() {
  //manualBTSensorChooser();
  receiveData();
  //lightSwitch();
  //storeIRCode();
  
}






void clearEEPROM() { //for testing, to clear all EEPROM data
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }
}

//prints every EEPROM Byte, including zero values
void printAllEEPROMBytes() {
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    Serial.println(EEPROM.read(i));
  }
}
//prints all longs stored with EEPROM_writeAnything(), but also includes zero values.
void printAllEEPROMAnything() {
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    long temp;
    EEPROM_readAnything(i, temp);
    Serial.println(temp);
  }
}


