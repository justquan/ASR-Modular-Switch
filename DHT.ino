//DHT module board pinout (white block facing you):
// VCC+ (3.3V - 6V power range)
// OUT (Digital pin), which is set up in dhtSwitch()
// GND

//receives the three char command from the interpretCommand(), and makes sense of it
void dhtInterpret(String filteredData) {
  if (filteredData.equals("__T")) {
    useTempNotHumidity = true;//so use temperature based switch
  }
  else if (filteredData.equals("__H")) {
    useTempNotHumidity = false; // so use humidity based switch
  }
  else {//if threshold number
    if (useTempNotHumidity) { //temp based switch
      tempThreshF = (int)convertCommandToLong(filteredData);//find an easier way to make it from string to int
      if (DEBUG) {
        Serial.print("tempThreshF=");
        Serial.println(tempThreshF);
      }
    }
    else {//if using humidity based switch
      humidityThresh = (int)convertCommandToLong(filteredData);//TODO: eventually differentiate between the values
      if (DEBUG) {
        Serial.print("humidityThresh=");
        Serial.println(humidityThresh);
      }
    }
  }
}


void dhtSwitch() {//TODO: make it so that there's a range, and option so that if it goes below OR above the temperature, trigger / dormant
  if (timeElapsed >= dhtSamplingPeriod) {
    timeElapsed = 0;
    float currentTempC = dht.getTemperature();
    float currentTempF = dht.toFahrenheit(currentTempC);
    String dhtStatus = dht.getStatusString();
    float currentHumidity = dht.getHumidity();
    if (DEBUG) {
      Serial.print("TempF: ");
      Serial.print(currentTempF);
      Serial.print(", TempC : ");
      Serial.print(currentTempC);
      Serial.print(", Humidity: ");
      Serial.println(currentHumidity);
    }
    if (dhtStatus.equals("OK")) {//ignore bad or not-a-number (nan) data from the library
      if (useTempNotHumidity) {///temperature based switch
        if (currentTempF >= tempThreshF) {
          trigger();
        }
        else {
          dormant();
        }
      }
      else {//humidity based switch
        if (currentHumidity >= humidityThresh) {
          trigger();
        }
        else {
          dormant();
        }
      }
    }
  }
}

//For debugging. The example program included with the DHT library  for printing status, temperature, and humidity put into a function
void exampleDHTPrint() {
  delay(dht.getMinimumSamplingPeriod());
  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();
  Serial.print(dht.getStatusString());//sometimes, the Status is "CHECKSUM", and the values are "nan"(not a number. When normal, the status is "OK".
  Serial.print("\t");
  Serial.print(humidity, 1);
  Serial.print("\t\t");
  Serial.print(temperature, 1);
  Serial.print("\t\t");
  Serial.println(dht.toFahrenheit(temperature), 1);
}

//only use when not using dhtSwitch()
void btPrintDHT() {
  if (timeElapsed >= dhtSamplingPeriod) {
    timeElapsed = 0;
    String msg;
    String dhtStatus = dht.getStatusString();
    float currentTempC = dht.getTemperature();
    float currentTempF = dht.toFahrenheit(currentTempC);
    float currentHumidity = dht.getHumidity();
    if (dhtStatus.equals("OK")) {
      if (useTempNotHumidity) {//if temp based switch
        String msg = String(currentTempF);
        msg.concat("F");
      }
      else {//
        msg = String(currentHumidity);
        msg.concat("%");
      }
      if (DEBUG) {
        Serial.println(msg);
      }
      BT.print(btSendBlock + msg);
    }
    else {
      if (DEBUG) {
        Serial.println("NAN val");
      }
    }
  }
}

