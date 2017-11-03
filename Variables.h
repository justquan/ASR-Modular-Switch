#include <IRremote.h>// public ir library

boolean relayState = false; //If false, relay is closed; if true, relay is open

//Pins
 int relayPin = 0; //D0, digital pin for relay control
 int lightPin = 0; //A0, analog pin for photoresistor (light) sensor
 int pirPin = 2; //D2, digital pin for Passive Infrared (PIR) sensor
 int tempPin = 2; // A2, analog pin for temperature sensor
 int IRpin = 11;  // D11, digital pin for IR sensor

//Variables from public ir library
 IRrecv irrecv(IRpin);
 decode_results results;

 //setting variables
int lightThresh = 450; // threshold value for light sensor, a higher value means there is more light
 
