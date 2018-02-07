//receives the three char command from the interpretCommand(), and makes sense of it
void smokeInterpret(String filteredData) {
  smokeThresh = (int)convertCommandToLong(filteredData);
}

void smokeSwitch() {
  if (timeElapsed >= smokeInterval) {//issue with this is that after 49 days, the timeElapsed will rollover. Need to find a system to combat this, perhaps resetting timeElapsed to a lower value when about to overflow.
    if (getSmokeValue >= smokeThresh) {//when there is smoke at levels that cross the threshold
      trigger();
      timeElapsed = 0;
      //      delay(10000);//find better way to set a delay
    }
    else {
      dormant();
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

