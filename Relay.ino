
//Close relay (connection is made) and changes relayState to false to match its status.
//First checks to make sure a change must be made in the state of the relay.
void closeRelay() {
  if (relayState) {
    digitalWrite(relayPin, HIGH);
    relayState = false;
    if (DEBUG) {
      Serial.println("Relay closed");
    }
  }
}

//Open relay (connection is broken) and changes relayState to true to match its status
//First checks to make sure a change must be made in the state of the relay.
void openRelay() {
  if (!relayState) {
    digitalWrite(relayPin, LOW);
    relayState = true;
    if (DEBUG) {
      Serial.println("Relay open");
    }
  }
}

//Function for reversing relay state. If relayState is true (open), close the relay, and if the relayState is false (closed), open the relay.
void reverseRelay() {
  if (relayState) {
    closeRelay();
  }
  else {
    openRelay();
  }
}

