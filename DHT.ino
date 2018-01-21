//DHT module board pinout (white block facing you):
// VCC+ (3.3V - 6V power range)
// OUT (Digital pin), set up in dhtSwitch()
// GND

//receives the three char command from the interpretCommand(), and makes sense of it
void dhtInterpret(String filteredData) {

}

void dhtSwitch() {
  if (firstSensorCall) {
    dht.setup(dhtPin);//sets up dht pin number
    firstSensorCall = false;
  }
  if (DEBUG) {
    exampleDHTPrint();
  }
}

//
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

