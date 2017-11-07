//TODO: implement changeable timer 
//ISSUE: still delays, false positives

//PIR Pinout:
//Lense facing away from you:
//Left Pin: 5V
//Middle Pin: Signal
//Right Pin: GND

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
  delay(300);
}

void motionSwitch(){
  if (isMotion){
    closeRelay();
    delay(motionDelay);//Not the best bethod because everything else stops when there's a delay
  }
  else{
    openRelay();
  }
}

