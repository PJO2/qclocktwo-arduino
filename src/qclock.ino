// ---------------------------------------------------
// a qlocktwo clone interface for arduino
// by Ph. Jounin 2020/11/04
// ---------------------------------------------------


#include <avr/power.h>
#include <avr/sleep.h>
#include <Wire.h>

#include "ds3231.h"
#include "photoresistor.h"
#include "buttons.h"
#include "leds.h"
#include "horloge.h"



// -----------------------------------------------
// wiring
const int MENU_PIN    = 2;       // interrupt pin for the menu button, AT hardware interruption PIN
const int PHOTO_IN    = 10;     // to the photoresistor for on demand VCC input
const int PHOTO_SENSOR= A0;    // second side of the photoresitor for measure
const int SCW_PIN     = 3;       // RTC SCW pin, this should be an ATmega hardware interruption PIN
const int HEARTBEAT   = 6;      // An optional led for a per second heartbeat
const int EDIT_PIN    = 4;       // pin for time edition
const int WS2812_PIN  = 8;     // toward ws2812 led strip

// implicit wiring :
// 3.3V to DS3231 VCC
// Arduino SDA pin (A4) to DS3231 SDA
// Arduino SCL pin (A5) to DS3231 SCL
// all grounds linked


// flags set by interruption routines
volatile bool flagDisplay, flagUI;

// ----------------------------------
// interruption : time and buttons
// ----------------------------------

// for now init a heartbeat each second
ISR(INT1_vect)
{
static int flap = LOW;
  flap = (flap==LOW ? HIGH : LOW);
  digitalWrite (HEARTBEAT, flap);
  flagDisplay = true;

}

static unsigned long lastMenuTime = -1;

ISR(INT0_vect)
{
     lastMenuTime = millis();
     flagUI = true;
} // ISR INT0





// ----------------------------------

void setup() {

  Serial.begin(9600);
   Wire.begin ();
   DS3231_reset ();   // stop current alarms, pins are SDA/SCL (architecture dependant) 
   PhotoResInit(PHOTO_IN, PHOTO_SENSOR); // Init photo resistor pins
   init_UI(MENU_PIN, EDIT_PIN);       // init pins used for UI 2 for INT0 and 5 because...
   
   leds_init (WS2812_PIN);      // init led strip WS2812

   init_Horloge (); // resolve words and position inside the clock 

   pinMode(HEARTBEAT, OUTPUT);  // a second count
   pinMode(7, OUTPUT);  // UI "debugger" 

   // Same as attachInterrupt(digitalPinToInterrupt(SCW_PIN), blink, FALLING);
   // attach hardware interrupt 0 and 1
   pinMode(SCW_PIN, INPUT_PULLUP);       // interrupt Pin 3 --> INT1
   EICRA   =     (1<<ISC01) | (1<<ISC00)    // Port 2 on rising signal
              |  (1<<ISC11) | (0<<ISC10);   // Port 3 on falling edge (DS3231 spec)
   EIFR |= (1<<INTF1) | (1<<INTF0);      // clear interrupt flags by writing 1 in the register bit control
   EIMSK   |= (1<<INT1) | (1<<INT0);     // enable Port 3 and Port 2 interrupts 

  DS3231_clearA1F();
  DS3231_setAlarmEverySecond();

  // enable deep sleep mode
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable ();

// DS3231_clearA2F();
// DS3231_setAlarmEveryMinute();

  DS3231_DisplayTimeToSerial(); // debug mode 

}



void loop() {
struct S_hrminsec  tm ; // 

  if (flagUI)
  {
    digitalWrite (7, HIGH);
    enter_UI();   // This will block for a while
    digitalWrite (7, LOW);
    flagUI = false;
    flagDisplay = true;
  }
  if (flagDisplay)
  {
     int bright = PhotoResDetect (PHOTO_IN, PHOTO_SENSOR);
     SetBrightness (bright);
     // Serial.println (bright);
   
     flagDisplay = false;
     DS3231_clearA1F();   // clear interrupt flags
     DS3231_clearA2F();
     tm = DS3231_getTime ();
     leds_showtime (tm.hours, tm.minutes, tm.seconds);
  }


 //  sleep_mode (); 
}
