#include "Variables.h";

//TODO: implement bluetooth
void setup() {
   Serial.begin(9600); //baud rate of 9600

   //setting pin modes
   pinMode(lightPin, INPUT);// TODO: possibly need to change to just constant pins based on numbers (like pin1 and pin2) instead of pins based on sensors because of the modular design
   pinMode(pirPin, INPUT);
   pinMode(relayPin, OUTPUT);
   
   irrecv.enableIRIn(); // Start the ir receiver

}

void loop() {
  //TODO: add code to select the right module index and have setting feature
  if (moduleIndex == 0){
    
  }
   else if (moduleIndex == 1){
    lightSwitch();
  }
   else if (moduleIndex == 2){
    
  }
  else if (moduleIndex == 3){
    motionSwitch();
  }
  else if (moduleIndex == 4){
   
  }
  else if (moduleIndex == 5){
    
  } 
  else if (moduleIndex == 6){
    temperatureSwitch(70);
  } 
  
  //TODO: create proper system to determine what module is being used to use appropriate methods in code
}
