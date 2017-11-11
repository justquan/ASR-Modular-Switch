#include <IRremote.h>// public ir library
#include <SoftwareSerial.h> //Needed for bluetooth serial data from wireless UART adaptor
#include <EEPROM.h> //for long term variables with saved values

boolean relayState = false; //If false, relay is closed; if true, relay is open

//Pins
 int relayPin = 1; //D1, digital pin for relay control
 int lightPin = 0; //A0, analog pin for photoresistor (light) sensor
 int pirPin = 2; //D2, digital pin for Passive Infrared (PIR) sensor
 int tempPin = 2; // A2, analog pin for temperature sensor
 int IRpin = 12;  // D12, digital pin for IR sensor
 int BTTX = 10; //D10, connect TX pin from BT module to this pin
 int BTRX = 11; //D11, connect RX pin from Bt module to this pin

//Variables from public ir library
 IRrecv irrecv(IRpin);
 decode_results results;

 //setting variables
int lightThresh = 400; //threshold value for light sensor, a higher value means there is more light
double tempThreshF = 60; //threshold value for temperature sensor in degrees Fahrenheit
int motionDelay = 60000; //time in milliseconds that the switch is closed after motion is detected until the sensing again

//Scaling constants
const double tempConversion = .48828125; //constant for lm35 sensor

//index variables

//bluetooth variables
SoftwareSerial BT(10,11); //makes a "virtual" serial port / UART, connect bt module TX to D10, and BT RX to D11, BT Vcc to 5v, GND to GND
char moduleIndex = '-1'; ////stores incoming character sent by bluetooth client, index variable determining which sensor is being used

//timer in milliseconds
int timer;
