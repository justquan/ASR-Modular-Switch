#include <IRremote.h>// public ir library
#include <SoftwareSerial.h> //Needed for bluetooth serial data from wireless UART adaptor
#include <EEPROM.h> //for long term variables with saved values
#include "EEPROMAnything.h"
#include "DHT.h"//library by markruys for the dht22
#include <elapsedMillis.h> // arduino library for elapsed time

boolean relayClosed = false; //If false, relay is open (off); if true, relay is closed (on)
boolean triggerStateIsClose = true; //If 0, relay is closed; if 1, relay is open; if 2, relay is strobing. triggerState is the state the switch

//Pins
int testLed = 1; //D1, for debugging
int relayPin = 2; //D2, digital pin for relay control
int photoresistorPin = 0; //A0, analog pin for photoresistor (light) sensor
int smokePin = 0; //A0, analog pin for MS-2 smoke detector
int pirPin = 3; //D3, digital pin for Passive Infrared (PIR) sensor
int tempPin = 2; // A2, analog pin for temperature sensor
int dhtPin = 3; //D3, digital pin for the DHT temperature and humidity module
int volPin = 0; //A0, analog pin for sound sensor
int IRpin = 9;  // D9, digital pin for IR sensor
int BTTX = 11; //D11, connect TX pin from BT module to this pin. Used to be 5, changed to 11 when soldered Pro mini.
int BTRX = 12; //D12, connect RX pin from BT module to this pin. Used to be 6, changed to 12 when soldered Pro mini.
int btPowerBasePin = 13; //D13, digital pin for the base of the 2N3904 NPN transistor used to control the GND to the HC-06. Used to be 8, changed to 13 when soldered to Pro mini.


//Variables from public ir library
IRrecv irrecv(IRpin);
decode_results results;

//setting variables
int lightThresh = 500; //threshold value for light sensor, a LOWER value means there is more light
double tempThreshF = 60; //threshold value for temperature sensor in degrees Fahrenheit
unsigned long motionInterval = 20000; // default time in milliseconds that the switch is closed after motion is detected until the sensing again
int smokeThresh = 400;//threshold before recognizing danger
int smokeInterval = 10000; //10 seconds, need to increase

//Scaling constants
const double tempConversion = .48828125; //constant for lm35 sensor

//index variables

//bluetooth variables
SoftwareSerial BT(BTTX, BTRX); //makes a "virtual" serial port / UART, connect bt module TX to D10, and BT RX to D11, BT Vcc to 5v, GND to GND
String command = ""; //placeholder string for bluetooth data

//stores last time motion was detected and not in the triggered not sensing state
unsigned long previousMotionMillis = 0;

boolean setupSwitch = true;//boolean to have bluetooth connection open for setting up the switch

int moduleIndex = -1; //index variable determining which sensor is being used

int IRcodeDEC = 0;

unsigned long storedCode;//int or unsigned long?

#define FNV_PRIME_32   16777619//from github ir demo program for hashing and saving ir code
#define FNV_BASIS_32   2166136261

/**
   EEPROM address to save IR code
*/
#define ADDR_CODE      0

boolean DEBUG = true;

boolean saveIR = false; //determines if switch is in save mode or not for the IR sensor

int normalVolume = -1; //saves the value from the sound sensor module at normal levels
int soundDifference = 2; //saves the difference from normalValue needed to register as a sound loud enough to trigger the switch

boolean useOneClap = true;//when soundSwitch() is run, detect one clap. Default state is true.
boolean useTwoClaps = false;//when soundSwitch() is run, detect two claps. Default state is false.

boolean strobing = false;//determines if the switch should be strobbing
boolean strobeIfTriggered = false;//determines whether or not switch should strobe if triggered
unsigned long strobeInterval = 1000; //the length of time in ms for how long the relay should stay remain in a state before switching during strobbing
unsigned long lastStrobeTime; //to track last time reversed for strobbing


boolean firstSensorCall = true;

DHT dht;
int dhtSamplingPeriod = dht.getMinimumSamplingPeriod();
int sendDataIntervalMillis = 3000;

//elapsedMillis is a long, so theoretically 49.7 days before rolling over
elapsedMillis timeElapsed; //always counts unless reset by making it equal to 0. Important: Only use this variable for one thing at a time. This can be used across multiple sensors, but only if the sensors are being used one at a time.

//relay analog value offsets, because relay when on affects analog values
int relayAnalogValOffsetSound = -6;//UNTESTED, NOT SURE IF 5// after testing 2/6, values first were 361 when closeRelay() and was 367for one second then 364 or 365 when openRelay(). Then, after 5 mins, range was 362 to 368/369. set as 6 on 2/6
int relayAnalogValOffsetLight = 5;//UNTESTED, NOT SURE IF 5
int relayAnalogValOffsetSmoke = 5;//UNTESTED, NOT SURE IF 5

//When there are two options to trigger / a max or min to trigger at
boolean triggerWhenOverLightValue = true;

//for storing IR Code in EEPROm 2/21
struct irEEPROMStruct{
  long irEEPROMCode;//should be in HEX form
  //use long not int so you won't run out of room
};

irEEPROMStruct irStored = {-1}; //default

