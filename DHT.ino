//dht sometimes doesn't work 3/4
//DHT module board pinout (white block facing you):
// VCC+ (3.3V - 6V power range)
// OUT (Digital pin), which is set up in dhtSwitch()
// GND

//receives the three char command from the interpretCommand(), and makes sense of it
void dhtInterpret(String filteredData) {
//  if (firstDHTCall) {
//    dht.setup(dhtPin);//sets up dht pin number
//    firstDHTCall = false;
//  }
  tempThreshF = (int)convertCommandToLong(filteredData);//find an easier way to make it from string to int
  if (DEBUG) {
    Serial.print("tempThreshF set to ");
    Serial.println(tempThreshF);
  }
}

void dhtSwitch() {//TODO: make it so that there's a range, and option so that if it goes below OR above the temperature, trigger / dormant
  //  if (DEBUG) {
  //    exampleDHTPrint();
  //  }
  if (timeElapsed >= dhtSamplingPeriod) {
    float currentTempC = dht.getTemperature();
    float currentTempF = dht.toFahrenheit(currentTempC);
    String dhtStatus = dht.getStatusString();
    if (dhtStatus.equals("OK")) {//ignore bad or not-a-number (nan) data from the library
      if (currentTempF >= tempThreshF) {
        trigger();
      }
      else {
        dormant();
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
void btPrintDHT() {//TODO: add utilities for humidity
  if (timeElapsed >= dhtSamplingPeriod) {
    timeElapsed = 0;
    String dhtStatus = dht.getStatusString();
    if (dhtStatus.equals("OK")) {
      String message = btSendBlock + String(dht.getTemperature());
      message.concat("C, ");
      message.concat(dht.getHumidity());
      message.concat("%");
      if (DEBUG) {
        Serial.println(message);
      }
      BT.print(message);
    }
    else {
      if (DEBUG) {
        Serial.println("NAN val");
      }
    }
  }
}

