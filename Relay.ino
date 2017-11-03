void closeRelay(){
  digitalWrite(relayPin, HIGH);
  relayState = false;
}

void openRelay() {
  digitalWrite(relayPin, LOW);
  relayState = true;
}

