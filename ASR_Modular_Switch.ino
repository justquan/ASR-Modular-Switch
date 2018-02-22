#include "Variables.h" //File with my global variables



void setup() {
  btPowerOn();  //When switch is rebooted, power flows to the HC-06 bluetooth module.
  Serial.begin(9600); //baud rate of 900
  BT.begin(9600);

  //setting pin modes
  pinMode(photoresistorPin, INPUT);// TODO: possibly need to change to just constant pins based on numbers (like pin1 and pin2) instead of pins based on sensors because of the modular design
  pinMode(pirPin, INPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(btPowerBasePin, OUTPUT);
  pinMode(testLed, OUTPUT);//debug
  //digitalWrite(testLed, HIGH);
  irrecv.enableIRIn(); // Start the ir receiver
  // Send test message to other device
  //BT.println("Hello from Arduino");
  if (DEBUG) {
    pinMode(LED_BUILTIN, OUTPUT);
  }
  //EEPROM_readAnything(0, storedCode);//reads whatever is at spot 0 in EEPROM and assigns value to storedCode
  //Serial.println(storedCode);//debug
  //printAllEEPROMAnything();
  // dht.setup(3);//when testing without bt app
  //  delay(2000);
  //  setNormalVolume();
  //  soundDifference = 5;
  //  btPowerOff();
  //  closeRelay();

  timeElapsed = 0; //reset time
  openRelay();//default state is opening the relay
  delay(100);
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
  //  btPowerOff();
  //  int sval = getSmokeValueRaw();
  //  Serial.println(sval);
  //  delay(200);
  //if(sval > 180 || sval < 180) {
  //  Serial.println(sval);
  //}

  //  btPowerOff();
  //  closeRelay();

  //  for (int i = 1; i < 20; i++) {
  //    Serial.println("Analog volume value when relay is closed / on : ");
  //    Serial.println(getVolumeAnalog());
  //    delay(200);
  //  }
  //
  //  openRelay();
  ////  Serial.println(getVolumeAnalogRaw());
  ////  delay(30);
  //  for (int i = 1; i < 20; i++) {
  //    Serial.println("Analog volume value when relay is open / off : ");
  //    Serial.println(getVolumeAnalog());
  //    delay(200);
  //  }


  //moduleIndex = 3;//for testing pir stuff
  //sensorChooser();
  //Serial.println(analogRead(0));
  //Serial.println(digitalRead(3));
  //delay(100);h

  //for testing sound sensor code
  //btPowerOff();
  //  volumeInterpret("__8");
  //  moduleIndex = 5;
  //  while(true) {
  //    sensorChooser();
  //  }
  //Serial.println(getVolumeAnalog());
  //delay(100);

  //relay being turned on and off + onboard LEDs disrupts analog signal of the sound sensor
}
