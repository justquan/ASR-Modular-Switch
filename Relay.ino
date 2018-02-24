
//Close relay (connection is made) and changes relayState to false to match its status.
//First checks to make sure a change must be made in the state of the relay.
void closeRelay() {
  digitalWrite(relayPin, LOW);//REPLACED WITH LOW ON 2/7
  relayClosed = true;
  if (DEBUG) {
    Serial.println("RELAY CLOSED");
  }
}

//Open relay (connection is broken) and changes relayState to true to match its status
//First checks to make sure a change must be made in the state of the relay.
void openRelay() {
  digitalWrite(relayPin, HIGH);//REPLACED WITH HIGH ON 2/7
  relayClosed = false;
  if (DEBUG) {
    Serial.println("RELAY OPEN");
  }
}

//Function for reversing relay state. If relayState is true (open), close the relay, and if the relayState is false (closed), open the relay.
void reverseRelay() {
  if (!relayClosed) {
    closeRelay();
  }
  else {
    openRelay();
  }
}

void trigger() {
  if (strobeIfTriggered) {
    if (!strobing) {
      strobing = true;
    }
  }
  else {
    if (strobing) {
      strobing = false;
    }
    if (triggerStateIsClose) {
      closeRelay();
    }
    else {
      openRelay();
    }
  }
}

//for when the switch is not triggered
//currently no way to strobe while dormant
void dormant() {
  if (strobing) {
    strobing = false;
  }
  if (triggerStateIsClose) {
    openRelay();
  }
  else {
    closeRelay();
  }
}

//checks if switch should be strobbing, and strobes it if it should in the main void loop()
void checkStrobe() {
  if (strobing) {
    unsigned long currentTime = millis();//TODO: use milliselapsed instead and do this for the pir interval
    if (currentTime - lastStrobeTime >= strobeInterval) {
      lastStrobeTime = currentTime;
      reverseRelay();
    }
  }
}
