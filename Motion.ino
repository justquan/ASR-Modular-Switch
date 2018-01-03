
//ISSUE: still delays, false positives

//PIR Pinout:
//Lense facing away from you:
//Left Pin: 5V
//Middle Pin: Signal
//Right Pin: GND

//receives the three char command from the interpretCommand(), and makes sense of it
void motionInterpret(String filteredData) {

}

boolean isMotion(){
  if(digitalRead(pirPin) == HIGH){
    return true;
  }
  else{
    return false;
  }
}

void printMotionData(){
  if(isMotion()){
    Serial.println("Motion detected!");
  }
  else{
    Serial.println("No motion");
  }
 // delay(300);
}

void motionSwitch(){
  printMotionData();
  if (isMotion){
    closeRelay();
    //delay(motionDelay);//Not the best bethod because everything else stops when there's a delay, need to change, use timer instead so switch can still sense for data and use other functions
  }
  else{
    openRelay();
  }
}

