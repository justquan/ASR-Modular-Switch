//TODO: write code to easily program IR signal to code (later do this with android app)


//TODO: learn how to make system to choose when setting the code and storing to eeprom or clearing eeprom
void infraredSwitch() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value, DEC); // Print the Serial 'results.value'
    irrecv.resume();   // Receive the next value
  }
}

