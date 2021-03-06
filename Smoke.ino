//still untested 3/4
//receives the three char command from the interpretCommand(), and makes sense of it

void smokeInterpret(String filteredData) {
  smokeThresh = (int)convertCommandToLong(filteredData);
}

void smokeSwitch() {
  if (timeElapsed >= smokeSamplingInterval) {
    timeElapsed = 0;
    int currentSmokeVal = getSmokeValue();
    errorDetection(currentSmokeVal);
    if (DEBUG) {
      Serial.println("Filtered smoke val:");
      Serial.println(currentSmokeVal);
    }
    if (timeElapsed2 >= smokeInterval) {//issue with this is that after 49 days, the timeElapsed will rollover. Need to find a system to combat this, perhaps resetting timeElapsed to a lower value when about to overflow.
      if (currentSmokeVal >= smokeThresh) {//when there is smoke at levels that cross the threshold
        trigger();
        timeElapsed2 = 0;
      }
      else {
        dormant();
      }
    }
  }
}

//gets analog smoke value; a higher value means a higher level of smoke.
int getSmokeValueRaw() {
  return analogRead(smokePin);
}

int getSmokeValue() {
  int alteredSmokeVal = getSmokeValueRaw();
  if (relayClosed) {
    return alteredSmokeVal + relayAnalogValOffsetSmoke;
  }
  else {
    return alteredSmokeVal;
  }
}

//for debugging
void printSmoke() {
  Serial.print("Raw smoke val:");
  Serial.println(getSmokeValueRaw());
}

void btPrintSmoke() {
  String msg = btSendBlock + getSmokeValue();
  BT.println(msg);
}

