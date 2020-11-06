// ---------------------------
// A small DS3231 TC clock manager
// Ph. Jounin  2020/11/03
//
// Implicit cabling is assumed:
//         - arduino SDA pin to DS3231 SDA
//         - arduino SCL pin to DS3231 SCL
// For Arduino nano :    SDA is A4, SCL is A5
//             ATTiny4 : SDA is PA6 (pin 7), SCL is PA4 (pin 9)
// ---------------------------

#include <Arduino.h>
#include <Wire.h>
#include "ds3231.h"

// BCD conversion routines (do not exist natively in the ATxx processors)
uint8_t _toBcd(uint8_t num)   { return ((num / 10) << 4) + (num % 10); }
uint8_t _fromBcd(uint8_t bcd) { return (10*((bcd&0xf0) >>4)) + (bcd & 0x0f); }


// read a single register (used to get control and status reg)
// follow the I2C standard : open the bus towards the RTC Clock
//                           write the address of the register to be read
//                           read the register
uint8_t DS3231_readRegister(uint8_t reg){
  uint8_t rc;
  Wire.beginTransmission(DS3231_ADDR);  
  Wire.write(reg);      
  Wire.endTransmission(false);
  Wire.requestFrom(DS3231_ADDR,1);    
  return Wire.read();       
}

// write a single register (used to modify control and status reg)
void DS3231_writeRegister(uint8_t reg, uint8_t data) {
  Wire.beginTransmission(DS3231_ADDR);  
  Wire.write(reg);                
  Wire.write(data);             
  Wire.endTransmission();           
}

// set the RTC clock 
// note the BCD operation since the DS3231 store the time data using BCD
void DS3231_setTime (int hours, int minutes, int seconds) {
  Wire.beginTransmission(DS3231_ADDR);  
  Wire.write(DS3231_SECONDS);
  Wire.write(_toBcd(seconds));
  Wire.write(_toBcd(minutes));
  Wire.write(_toBcd(hours) | DS3231_24H);
  Wire.endTransmission();           
}


// read the RTC clock 
struct S_hrminsec DS3231_getTime ()
{
struct S_hrminsec tm;  
  Wire.beginTransmission(DS3231_ADDR);  
  Wire.write(DS3231_SECONDS);
  Wire.endTransmission(false);
  Wire.requestFrom(DS3231_ADDR,3);    
  tm.seconds = _fromBcd (Wire.read ());
  tm.minutes = _fromBcd (Wire.read ());
  tm.hours   = _fromBcd (Wire.read () & ~DS3231_24H );
  return tm;
}

// Program the RTC clock to send an alarm every second
// process described in the datasheet https://datasheets.maximintegrated.com/en/ds/DS3231.pdf
// Figure 1 page 11
void DS3231_setAlarmEverySecond() {
  int ctrl_reg = 0;
  Wire.beginTransmission(DS3231_ADDR);  
  Wire.write(DS3231_ALARM1_SECONDS);                
  Wire.write(0x80);
  Wire.write(0x80);
  Wire.write(0x80);
  Wire.write(0x80);
  Wire.endTransmission();           

  ctrl_reg = DS3231_readRegister(DS3231_CONTROL);
  DS3231_writeRegister(DS3231_CONTROL, ctrl_reg | 0x05);
} // DS3231_setAlarmEverySecond


// Same for every minute
void DS3231_setAlarmEveryMinute() {
  int ctrl_reg = 0;
  Wire.beginTransmission(DS3231_ADDR);  
  Wire.write(DS3231_ALARM2_MINUTES);                
  Wire.write(0x80);
  Wire.write(0x80);
  Wire.write(0x80);
  Wire.endTransmission();           

  ctrl_reg = DS3231_readRegister(DS3231_CONTROL);
  DS3231_writeRegister(DS3231_CONTROL, ctrl_reg | 0x06);
} // DS3231_setAlarmEveryMinute



// Clear the interruption flag from alarm 1
// process described in the datasheet page 14 of
// https://datasheets.maximintegrated.com/en/ds/DS3231.pdf
// BEWARE: Wire library is using interruptions, this procedure should
// NOT be called inside an interruption but in the main loop

void DS3231_clearA1F()
{
  int status_reg = DS3231_readRegister(DS3231_STATUS);
  DS3231_writeRegister(DS3231_STATUS, status_reg & ~0x01);
}


void DS3231_clearA2F()
{
  int status_reg = DS3231_readRegister(DS3231_STATUS);
  DS3231_writeRegister(DS3231_STATUS, status_reg & ~0x02);
}

void DS3231_reset ()
{
  DS3231_writeRegister(DS3231_CONTROL,0);
  DS3231_writeRegister(DS3231_STATUS,0);
}


void DS3231_DisplayTimeToSerial()
{
char buf[128];
struct S_hrminsec tm = DS3231_getTime ();
   sprintf (buf, "il est %d heures, %d minutes et %d seconds", tm.hours, tm.minutes, tm.seconds);
   Serial.println (buf);
   delay (100);
} // DS3231_DisplayTimeToSerial
