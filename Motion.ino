//TODO: implement changeable timer 

boolean isMotion(){
  if(digitalRead(pirPin) == HIGH){
    return true;
  }
  else{
    return false;
  }
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

