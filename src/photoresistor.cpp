// ----------------------------------------
// measure the photo resistor voltage 
// Wiring is : Arduino PhotoVcc Pin -- photoresistor --- 1kOhm resistor -- GND
//                                                    | 
//                                          Arduino PhotoSensor Pin
//
// ----------------------------------------

#include <Arduino.h>
#include "photoresistor.h"

#define NB_VALS 6
#define BRIGHTNESS_MULTIPLIER 6

static int lightCal[NB_VALS];
static int read_idx=0;
static int sum;

void PhotoResInit (int VccPin, int SensorPin)
{
  pinMode(VccPin,   OUTPUT);
  pinMode(SensorPin, INPUT);   

  // fill buffer
  for (int ark=0 ; ark<NB_VALS ; ark ++)
     PhotoResDetect (VccPin, SensorPin);
} // PhotoResInit 


int PhotoResDetect (int VccPin, int SensorPin)
{

  digitalWrite (VccPin, HIGH);       // Activate current just for a while
  delay (50);                       // wait for stabilisation (30ms according to datasheet)
  sum -= lightCal[read_idx];
  lightCal[read_idx] = analogRead(SensorPin); // measure
  sum += lightCal[read_idx];    // replace old value
  
  digitalWrite (VccPin, LOW);        // Do not waste power 
  
  read_idx++;
  read_idx %= NB_VALS;

  int bright =  sqrt ( sum / NB_VALS ) * BRIGHTNESS_MULTIPLIER;
  return bright > 255 ? 255 : bright ;
} // PhotoResDetect
