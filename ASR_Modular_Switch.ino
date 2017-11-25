#include "Variables.h" //File with my global variables

//TODO: implement bluetooth
void setup() {
  Serial.begin(9600); //baud rate of 9600
  BT.begin(9600); // interferes serial begin?? TODO: test this with bluetooth module

  //setting pin modes
  pinMode(lightPin, INPUT);// TODO: possibly need to change to just constant pins based on numbers (like pin1 and pin2) instead of pins based on sensors because of the modular design
  pinMode(pirPin, INPUT);
  pinMode(relayPin, OUTPUT);

  irrecv.enableIRIn(); // Start the ir receiver
  // Send test message to other device
  BT.println("Hello from Arduino");
  timer = 0;
    pinMode(LED_BUILTIN, OUTPUT);//for testing from instructables

}

void loop() {
  manualBTSensorChooser();
}
