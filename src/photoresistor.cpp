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
#define BRIGHTNESS_MULTIPLIER 6     // may be increased to increase brightness

static int lightCal[NB_VALS];       // a circular buffer to save previous measures
static int read_idx=0;              // current position in the circular buffer
static int sum;                     // the sum of the circular buffer

void initPhotoRes (int VccPin, int SensorPin)
{
  pinMode(VccPin,   OUTPUT);
  pinMode(SensorPin, INPUT);   

  // fill buffer
  for (int ark=0 ; ark<NB_VALS ; ark ++)
     detectPhotoRes (VccPin, SensorPin);
} // initPhotoRes


int detectPhotoRes (int VccPin, int SensorPin)
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
} // detectPhotoRes
