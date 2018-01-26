//DHT module board pinout (white block facing you):
// VCC+ (3.3V - 6V power range)
// OUT (Digital pin), set up in dhtSwitch()
// GND

//receives the three char command from the interpretCommand(), and makes sense of it
void dhtInterpret(String filteredData) {
  tempThreshF = (int)convertCommandToLong(filteredData);//find an easier way to make it from string to int

  if (DEBUG) {
    Serial.println("Threshold temperature in Fahrenheit set to ");
    Serial.print(tempThreshF);
    Serial.println();
  }
}

void dhtSwitch() {//TODO: make it so that there's a range, and option so that if it goes below OR above the temperature, trigger / dormant
  if (firstSensorCall) {
    dht.setup(dhtPin);//sets up dht pin number
    firstSensorCall = false;
  }
  if (DEBUG) {
    exampleDHTPrint();
  }
  float currentTempC = dht.getTemperature();
  float currentTempF = dht.toFahrenheit(currentTempC);
  String dhtStatus = dht.getStatusString();
  if (dhtStatus.equals("OK")) {//ignore bad or not-a-number data from the library //TODO: debug and check this
    if (currentTempF >= tempThreshF) {
      trigger();
    }
    else {
      dormant();
    }
  }
  delay(dht.getMinimumSamplingPeriod());
}

void exampleDHTPrint() {
  delay(dht.getMinimumSamplingPeriod());

  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();

  Serial.print(dht.getStatusString());
  Serial.print("\t");
  Serial.print(humidity, 1);
  Serial.print("\t\t");
  Serial.print(temperature, 1);
  Serial.print("\t\t");
  Serial.println(dht.toFahrenheit(temperature), 1);
}

