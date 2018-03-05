#include "Variables.h" //File with my global variables
//NOTE: Switch stops working when dynamic memory use by the global variables goes above ~80%. Limit debugging strings
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
    Serial.print("Last target DEC IR: ");
    Serial.println(irStored.irEEPROMCode);
  }
  openRelay();//default state is opening the relay
  statusLEDOff();

  delay(setupDelay);//time for relay to open
  timeElapsed = 0; //reset time
  timeElapsedSendBT = 0;
  setClockSpeedDefault();//causing issues before
  greenStatusLEDOn();
  dht.setup(dhtPin);//sets up dht pin number
  //don't know if it conflicts yet with others modules yet if you just set up at the start
}

void loop() {
  if (setupSwitch) { //if the switch is in the setup mode
    receiveData();//then receive data from the Android
    sendData();//super buggy, add delays. Also sometimes makes app slow. Make it so that it doesn't send messages until android app sends arduino a msg indicating that it's finished connecting maybe?// TODO: 3/1. FIX! sometimes sends data every 3 seconds, but for some modules, like motion, only changes when there is a change in state. make all of them change only every 3 secs.
    //TODO: fix sendData so that it is called individually in each sensor tab after new data is received, not run it consistently
    runSetupProcesses();
  }
  else if (!strobing) { //so once the switch is strobing, it is strobing forever until reset
    sensorChooser();//if the switch isn't in setup mode anymore, run method for the correct sensor in a loop.
  }
  checkStrobe();
}
