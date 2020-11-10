// ---------------------------------------------------
// a qlocktwo clone interface for arduino
// by Ph. Jounin 2020/11/04
// ---------------------------------------------------

#include <Arduino.h>
#include <avr/power.h>
#include <avr/sleep.h>

#include "logger.h"   // to log or not to log
#include "ds3231.h"
#include "photoresistor.h"
#include "buttons.h"
#include "leds.h"
#include "horloge.h"



// -----------------------------------------------
// wiring
const int MENU_PIN    = 2;       // interrupt pin for the menu button, AT hardware interruption PIN
const int SCW_PIN     = 3;       // RTC SCW pin, this should be an ATmega hardware interruption PIN
const int PHOTO_IN    = 10;     // to the photoresistor for on demand VCC input
const int PHOTO_SENSOR= A0;    // second side of the photoresitor for measure
const int HEARTBEAT   = 6;      // An optional led for a per second heartbeat
const int EDIT_PIN    = 4;       // pin for time edition
const int WS2812_PIN1 = 8;     // toward ws2812 led strip for hour
const int WS2812_PIN2 = 9;     // toward ws2812 led strip for minutes
const int UI_PIN      = 7 ;    // an indicator that we are in setup mode
// implicit wiring :
// 3.3V to DS3231 VCC
// Arduino SDA pin (A4) to DS3231 SDA
// Arduino SCL pin (A5) to DS3231 SCL
// all grounds linked



// ----------------------------------
// interruption : time and buttons
// ----------------------------------
// flags set by interruption routines
volatile bool flagDisplay, flagUI;

// for now init a heartbeat each second and tag display should change
ISR(INT1_vect)
{
static int flap = LOW;
  // do an heartbeat control
  flap = (flap==LOW ? HIGH : LOW);
  digitalWrite (HEARTBEAT, flap);
  // ask for a refresh
  flagDisplay = true;
} // ISR INT1


ISR(INT0_vect)
{
     flagUI = true;  // will enter in MenuMode
} // ISR INT0


// ----------------------------------
// Setups
// ----------------------------------

// Enable interruptions pin2 and 3 (hardware) 
// and set deep sleep mode
static void initNanoHardware()
{
   // Same as attachInterrupt(digitalPinToInterrupt(SCW_PIN), blink, FALLING);
   // attach hardware interrupt 0 and 1
   // cf atmega328P datasheet chapter 12 page 54
   // https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf
   EICRA   =     (1<<ISC01) | (1<<ISC00)    // Port 2 on rising signal
              |  (1<<ISC11) | (0<<ISC10);   // Port 3 on falling edge (DS3231 spec)
   EIFR    |= (1<<INTF1) | (1<<INTF0);      // clear interrupt flags by writing 1 in the register bit control
   EIMSK   |= (1<<INT1) | (1<<INT0);        // enable Port 3 and Port 2 interrupts 

  // enable deepest sleep mode
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable ();
} // initNanoHardware

void setup() 
{   
   // Each module has its own init function 
   initLog (9600);                         // logger
   initDS3231 ();                          // stop current alarms, pins are SDA/SCL (architecture dependant) 
   initPhotoRes(PHOTO_IN, PHOTO_SENSOR);   // Init photo resistor pins
   init_UI(MENU_PIN, EDIT_PIN, UI_PIN);    // init pins used for UI 2 for INT0 and 5 because...
   initLeds (WS2812_PIN1, WS2812_PIN2);   // init led strip WS2812
   init_Horloge ();                        // in fact does nothing 

   pinMode(HEARTBEAT, OUTPUT);           // a second count
   pinMode(SCW_PIN, INPUT_PULLUP);       // interrupt Pin 3 --> INT1

   initNanoHardware();                   // init nano
   DS3231_setAlarmEverySecond ();        // enable wake up routine 
   flagDisplay = true;                   // display time at startup
}  // Setup


// ----------------------------------
// Main Loop
// flagUI set by RTC clock  --> display time
// flagDisplay  set by menu button --> enter set time mode
// ----------------------------------

void showTime ()
{
struct S_hrminsec  tm ; 
int bright;
static uint8_t last_minutes = -1;

     // adjust brightness
     bright = detectPhotoRes (PHOTO_IN, PHOTO_SENSOR);
     setBrightnessLeds (bright);
     Log (logDEBUG, F("brightness: %d"), bright);

     tm = DS3231_getTime ();   // get current time
     if (tm.minutes != last_minutes) // time has changed ?
     {
        // display time
        showtimeLeds (tm.hours, tm.minutes, tm.seconds);
        // provide dome log if needed
        DS3231_DisplayTimeToSerial (& tm);
        last_minutes = tm.minutes;
     }
} // ShowTime


void loop() 
{
  if (flagUI)
  {
    enter_UI();           // This may block for a while
    flagUI      = false;  // acknoledge interrupt
    flagDisplay = true;   // force new display
  }
  if (flagDisplay)
  {
     showTime ();          // just above 
     flagDisplay = false;  // acknowledge interrupt for nano
     DS3231_clearA1F();    // clear DS3231 interrupts flags
     DS3231_clearA2F();
    
     sleep_mode ();        // job done, wait for next second/minute
  } // RTC interrupt has fired
} // loop forever
