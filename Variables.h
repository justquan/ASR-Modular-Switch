#include <IRremote.h>// public ir library
#include <SoftwareSerial.h> //Needed for bluetooth serial data from wireless UART adaptor
#include <EEPROM.h> //for long term variables with saved values
#include "EEPROMAnything.h"
#include "DHT.h"//library by markruys for the dht22
#include <elapsedMillis.h> // arduino library for elapsed time
#include <avr/sleep.h>//for sleep functions

//Pins
int moduleAnalogPin = 0;//The pins for modules using the soldered board
int moduleDigitalPin = 10;

int relayPin = 2; //D2, digital pin for relay control
int greenStatusLED = 5;//green digital pin for RG led
int redStatusLED = 6;//red digital pin for RG led
int photoresistorPin = moduleAnalogPin; //A0, analog pin for photoresistor (light) sensor
int smokePin = moduleAnalogPin; //A0, analog pin for MS-2 smoke detector
int pirPin = moduleDigitalPin; //D3, digital pin for Passive Infrared (PIR) sensor
int tempPin = moduleAnalogPin; // A2, analog pin for temperature sensor
int dhtPin = moduleDigitalPin; //D3, digital pin for the DHT temperature and humidity module
int volPin = moduleAnalogPin; //A0, analog pin for sound sensor
int IRpin = moduleDigitalPin;  // D9, digital pin for IR sensor
int BTTX = 11; //D11, connect TX pin from BT module to this pin. Used to be 5, changed to 11 when soldered Pro mini.
int BTRX = 12; //D12, connect RX pin from BT module to this pin. Used to be 6, changed to 12 when soldered Pro mini.
int btPowerBasePin = 13; //D13, digital pin for the base of the 2N3904 NPN transistor used to control the GND to the HC-06. Used to be 8, changed to 13 when soldered to Pro mini.

//Relay booleans
boolean relayClosed = false; //If false, relay is open (off); if true, relay is closed (on)
boolean triggerStateIsClose = true; //If 0, relay is closed; if 1, relay is open; if 2, relay is strobing. triggerState is the state the switch

//Variables from public ir library
IRrecv irrecv(IRpin);
decode_results results;

//DHT
DHT dht;

//Sensor setting variables
int lightThresh = 500; //threshold value for light sensor, a LOWER value means there is more light
double tempThreshF = 60; //threshold value for temperature sensor in degrees Fahrenheit
unsigned long motionInterval = 20000; // default time in milliseconds that the switch is closed after motion is detected until the sensing again
int smokeThresh = 400;//threshold before recognizing danger
int smokeInterval = 10000; //10 seconds, need to increase

//time variables
int clockSpeedReduction = 0x01; //1/2 speed
int setupDelay = 100;
int minTimeBeforeSleep = 300000;//if switch is in setup mode for  300,000 ms or 5 mins, it goes to sleep and needs to be reset to be used.
int dhtSamplingPeriod = dht.getMinimumSamplingPeriod();
int sendDataIntervalMillis = 3000;//data sent to android every 3000ms
int soundTriggerDelay = 1000;//after sound sensor is triggered, how long it waits before sensing agian
int twoClapWait1 = 150;//for two clap function of sound sensor
int twoClapWait2 = 500;//for two clap function of sound sensor
int btDisconnectDelay = 1000;//gives time to turn off to prevent interference in analog signals. Not affected by clock slowing, so no need to scale.


//Scaling constants
const double tempConversion = .48828125; //constant for lm35 sensor

//index variables
int moduleIndex = -1; //index variable determining which sensor is being used

//bluetooth variables
SoftwareSerial BT(BTTX, BTRX); //makes a "virtual" serial port / UART, connect bt module TX to D10, and BT RX to D11, BT Vcc to 5v, GND to GND
String command = ""; //placeholder string for bluetooth data

boolean btConnectionMade = false;//switches to true if one is made
boolean setupSwitch = true;//boolean to have bluetooth connection open for setting up the switch
boolean DEBUG = true;
boolean sendDataToAndroid = true;//boolean determining whether or not switch should try sending data to the Android via BT.
boolean saveIR = true; //determines if switch is in save mode or not for the IR sensor. feb 22, made it so that the defualt is true

int normalVolume = -1; //saves the value from the sound sensor module at normal levels
int soundDifference = 6; //saves the difference from normalValue needed to register as a sound loud enough to trigger the switch

boolean useOneClap = false;//when soundSwitch() is run, detect one clap. Default state is false.
boolean useTwoClaps = true;//when soundSwitch() is run, detect two claps. Default state is true.

boolean strobing = false;//determines if the switch should be strobbing
boolean strobeIfTriggered = false;//determines whether or not switch should strobe if triggered
unsigned long strobeInterval = 1000; //the length of time in ms for how long the relay should stay remain in a state before switching during strobbing
unsigned long lastStrobeTime; //to track last time reversed for strobbing

boolean firstSensorCall = true;
boolean firstDHTCall = true;

//elapsedMillis is a long, so theoretically 49.7 days before rolling over
elapsedMillis timeElapsed; //always counts unless reset by making it equal to 0. Important: Only use this variable for one thing at a time. This can be used across multiple sensors, but only if the sensors are being used one at a time.
elapsedMillis timeElapsedSendBT;//timeElapsed but specifically for sending data via BT to Android.

//relay analog value offsets, because relay when on affects analog values
int relayAnalogValOffsetSound = -6;//UNTESTED, NOT SURE IF 5// after testing 2/6, values first were 361 when closeRelay() and was 367for one second then 364 or 365 when openRelay(). Then, after 5 mins, range was 362 to 368/369. set as 6 on 2/6
int relayAnalogValOffsetLight = 5;//UNTESTED, NOT SURE IF 5
int relayAnalogValOffsetSmoke = 5;//UNTESTED, NOT SURE IF 5

//When there are two options to trigger / a max or min to trigger at
boolean triggerWhenOverLightValue = true;

//for storing IR Code in EEPROM 2/21
struct storedSettingsStruct {
  long irEEPROMCode;//should be in DEC form, use long not int so you won't run out of room
};

storedSettingsStruct irStored = {0}; //default
