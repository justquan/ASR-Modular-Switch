#include "Variables.h" //File with my global variables

void setup() {
  btPowerOn();  //When switch is rebooted, power flows to the HC-06 bluetooth module.
  Serial.begin(9600); //baud rate of 900
  BT.begin(9600);//Starts BT module

  //setting pin modes
  pinMode(moduleDigitalPin, INPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(redStatusLED, OUTPUT);
  pinMode(greenStatusLED, OUTPUT);
  pinMode(btPowerBasePin, OUTPUT);//pin to transistor controlling the power of the BT module

  irrecv.enableIRIn(); // Start the ir receiver

  EEPROM_readAnything(0, irStored);//sets irStored struct to have previously saved value in the EEPROM. Needed for infraredSwitch()
  if (DEBUG) {
    Serial.print("Previously stored IR code in EEPROM in DEC form: ");
    Serial.println(irStored.irEEPROMCode);
  }
  openRelay();//default state is opening the relay
  statusLEDOff();

  delay(setupDelay);//time for relay to open
  timeElapsed = 0; //reset time
  timeElapsedSendBT = 0;
  setClockSpeedDefault();
}

void loop() {
  if (setupSwitch) { //if the switch is in the setup mode
    receiveData();//then receive data from the Android
    //    sendData();//super buggy, add delays. Also sometimes makes app slow. Make it so that it doesn't send messages until android app sends arduino a msg indicating that it's finished connecting maybe?
    //TODO: fix sendData so that it is called individually in each sensor tab after new data is received, not run it consistently
    runSetupProcesses();
  }
  else if (!strobing) { //so once the switch is strobing, it is strobing forever until reset
    sensorChooser();//if the switch isn't in setup mode anymore, run method for the correct sensor in a loop.
  }
  checkStrobe();
}
