int printLightData(){
  int lightData = analogRead(lightPin);
  Serial.println("lightData: " + lightData);
  return lightData;
}

void lightSwitch(){
  if (printLightData() < lightThresh){
    openRelay();
  }
  else{
    closeRelay(); //open and closed must be closed might be reversed
  }
}

