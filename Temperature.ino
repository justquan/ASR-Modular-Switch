
//converts current raw data into current temperature in Celcius and returns it
double getTempC(){
  double temp = analogRead(tempPin);
  temp *= tempConversion;
  return temp;
}

//converts current raw data into current temperature in Fahrenheit and returns it
double getTempF(){
  double temp = getTempC();//gets temperature in celcius
  temp *= 9/5;
  temp += 32;
  return temp;
}

// gathers and prints raw data from temperature sensor and prints temperature in Celcius and Fahrenheit
void printTempData(){
  double tempRaw = analogRead(tempPin);
  double tempC = getTempC();
  double tempF = getTempF();
  Serial.print("RAW DATA: ");
  Serial.print (tempRaw);
  Serial.println(" ");
  Serial.print("CELSIUS: ");
  Serial.print(getTempC());
  Serial.println("*C ");
  Serial.print("FAHRENHEIT: ");
  Serial.print(getTempF());
  Serial.println("*F");
}

void temperatureSwitch(double threshold){//can also just use global variable tempThreshF
  if (getTempF() < threshold){
    openRelay();
  }
  else{
    closeRelay();
  }
}

