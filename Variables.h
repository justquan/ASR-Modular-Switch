#include <IRremote.h>// public ir library
#include <SoftwareSerial.h> //Needed for bluetooth serial data from wireless UART adaptor
#include <EEPROM.h> //for long term variables with saved values
#include "EEPROMAnything.h"


boolean relayState = false; //If false, relay is closed; if true, relay is open
int triggerState = 1; //If 0, relay is closed; if 1, relay is open; if 2, relay is strobing. triggerState is the state the switch

//Pins
int testLed = 5; //D5, for debugging
int relayPin = 1; //D1, digital pin for relay control
int photoresistorPin = 0; //A0, analog pin for photoresistor (light) sensor
int pirPin = 2; //D2, digital pin for Passive Infrared (PIR) sensor
int tempPin = 2; // A2, analog pin for temperature sensor
int IRpin = 9;  // D9, digital pin for IR sensor
int BTTX = 10; //D10, connect TX pin from BT module to this pin
int BTRX = 11; //D11, connect RX pin from Bt module to this pin

//Variables from public ir library
IRrecv irrecv(IRpin);
decode_results results;

//setting variables
int lightThresh = 500; //threshold value for light sensor, a LOWER value means there is more light
double tempThreshF = 60; //threshold value for temperature sensor in degrees Fahrenheit
int motionDelay = 60000; //time in milliseconds that the switch is closed after motion is detected until the sensing again

//Scaling constants
const double tempConversion = .48828125; //constant for lm35 sensor

//index variables

//bluetooth variables
SoftwareSerial BT(10, 11); //makes a "virtual" serial port / UART, connect bt module TX to D10, and BT RX to D11, BT Vcc to 5v, GND to GND
String command = ""; //placeholder string for bluetooth data

//timer in milliseconds
boolean setupSwitch = true;//boolean to have bluetooth connection open for setting up the switch, TODO: implement a timer or some type of system for only receiving data from the Android at the beginning for a certain amount of time.
int timer;

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
