#include "Variables.h" //File with my global variables
//NOTE: Switch stops working when dynamic memory use by the global variables goes above ~80%. Limit debugging strings
void setup() {
  btPowerOn();  //When switch is rebooted, power flows to the HC-06 bluetooth module.
  Serial.begin(9600); //baud rate of 9600
  BT.begin(9600);//Starts BT module

  //setting pin modes
  pinMode(moduleDigitalPin, INPUT);
  pinMode(moduleDigitalPin, INPUT_PULLUP);//enables integrated pull up resistor on digital pin
  digitalWrite(A0, INPUT_PULLUP);  // set pullup on analog pin 0 for the analog module pin
  pinMode(relayPin, OUTPUT);
  pinMode(redStatusLED, OUTPUT);//declares pin as being digital outputs for red node of RG LED
  pinMode(greenStatusLED, OUTPUT);//declares pin as being digital outputs for green node of RG LED
  pinMode(btPowerBasePin, OUTPUT);//pin to transistor controlling the power of the BT module

  irrecv.enableIRIn(); // Starts the ir receiver, which is used if IR is selected
  dht.setup(dhtPin);// Starts the dht, which is used if temperature / humidity is selected


  EEPROM_readAnything(0, irStored);//sets irStored struct to have previously saved value in the EEPROM. Needed for infraredSwitch()
  if (DEBUG) {
    Serial.print("Last target DEC IR: ");
    Serial.println(irStored.irEEPROMCode);
  }
  openRelay();//default state is opening the relay
  statusLEDOff();
  delay(setupDelay);//time for relay to open

  //reset elapsed times
  timeElapsed = 0;
  timeElapsedSendBT = 0;

  setClockSpeedDefault();//in case previously set to a slower clock speed

  //led status sequence to indicate that switch has been reset
  redStatusLEDOn();
  delay(statusLEDDelay);
  statusLEDOff();
  greenStatusLEDOn();
  delay(statusLEDDelay);
  statusLEDOff();
}

void loop() {
  if (setupSwitch) { //if the switch is in the setup mode
    receiveData();//then receive data from the Android
    sendData();
    runSetupProcesses();
  }
  else if (!strobing) { //Once the switch is strobing, it is strobing forever until manually reset
    sensorChooser();//if the switch isn't in setup mode anymore, run method for the correct sensor in a loop.
  }
  checkStrobe();
}
