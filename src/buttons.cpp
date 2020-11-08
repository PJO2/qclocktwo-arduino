// ---------------------------------
// set time based on a two buttons user interface :
//   - The first button control the parameter which is to be edited (hour then minute)
//   - The second one increase the current parameter
// ---------------------------------

#include <Arduino.h>
#include "ds3231.h"
#include "logger.h"
#include "leds.h"

#define POLLING_TIMER 100  // scans button every 100ms

// if nothing happens during one minute 
#define UI_TIMEOUT 60000l 

static unsigned long lastActionTime ; // for timeout

enum e_buttons  { MODE, EDIT, NB_BUTTONS };    // button names
enum e_Action   { EDIT_NONE, EDIT_HOURS, EDIT_MINUTES, EDIT_LAST };  // param currenlty edited
enum e_States   { PRESSED, RELEASED };         // button states

// the button status
static struct S_Button 
{
  int            pin;
  enum e_States  state;
  bool           pressed;
}
tButtons[NB_BUTTONS];

static int signalPin;

// ------------------------------------
// init the "User Interface"
// note that the modePin can not been changed since it uses the hardware interrupt pin 
void init_UI (int modePin, int editPin, int uiPin)
{
  // record buttons status
  tButtons[MODE].pin = modePin;
  tButtons[EDIT].pin = editPin;
  tButtons[MODE].state   = tButtons[EDIT].state   = RELEASED;
  tButtons[MODE].pressed = tButtons[EDIT].pressed = false;

  // activate input on the pins
  pinMode (modePin, INPUT);
  pinMode (editPin, INPUT);
  pinMode (uiPin, OUTPUT);
  signalPin = uiPin;
} // init_UI


// ----------------------------------
// utility function to manage button states and timeout counter
// return the index of the pressed button or -1 if no change
// ----------------------------------
static int manage_UI ()
{
int ark;
    for (ark=NB_BUTTONS-1 ; ark>=0 ; ark--)
    {
      if ( digitalRead (tButtons[ark].pin)==LOW  && tButtons[ark].state==PRESSED  )
      {
         Log (logWARN, F("Action: release button"));
         tButtons[ark].state  = RELEASED;
      }
      if ( digitalRead (tButtons[ark].pin)==HIGH && tButtons[ark].state==RELEASED )
      {
         Log (logWARN, F("Action: release button"));
         tButtons[ark].state   = PRESSED;
         tButtons[ark].pressed = true;
         lastActionTime = millis();   // reset timeout
         break;    // <-- assume that two buttons are not pressed in the same 1/10s
      }
    }
    return ark;
} // manage_UI

// display with blinking effect
static void blinking_display (int action, const struct S_hrminsec *tm)
{
static int count=0;                  // count loop iterations for blinking effect
    count ++;
    showtimeLeds ( (action==EDIT_HOURS   && count %3==0) ?  -1 : tm->hours,
                    (action==EDIT_MINUTES && count %3==0) ?  -1 : tm->minutes, 
                    tm->seconds);
} // blinking_display 

// ----------------------------------
// adjust hours first then minutes
// Since enter_UI has been called, hours is already selected
// note that this function returns to the main loop only at the end 
// ----------------------------------
void enter_UI ()
{
int  action = EDIT_NONE;      // current action : edit hours
struct S_hrminsec tm; 
int    idx;                    // button activated

  digitalWrite (signalPin, HIGH);   // highlight a led to tag menu mode 

  tm = DS3231_getTime ();         // read current time before update
  lastActionTime = millis();      // A poor man's timeout

  tButtons[MODE].state=RELEASED;  // force new mode action

  // do it until timeout or   [ mode bouton released and last action done ]
  while(       millis() - lastActionTime < UI_TIMEOUT                       // timeout
         &&  !(tButtons[MODE].state==RELEASED  && action==EDIT_LAST) )      // last action terminated
  {    
    delay (POLLING_TIMER);        // debounce for buttons (and blinking)
    idx = manage_UI (); // manage button status, return idx of a just pressed button 
    // button processing starts here 
    switch (idx)
    {
      case MODE : 
           action++;  // next parameter edition
           break;
      case EDIT :
           switch (action) 
           {
             case EDIT_HOURS :    
                      tm.hours = (tm.hours+1) % 24;
                      break;                   
             case EDIT_MINUTES : 
                      tm.minutes = (tm.minutes+1) % 60;
                      tm.seconds = 0;
                      break;         
           }
           DS3231_setTime (tm.hours, tm.minutes, tm.seconds );
           break;
    } // fullfill button actions

    blinking_display (action, & tm);   // display blinking leds for parameter under edition
  } // until done or timeout
  
  delay (POLLING_TIMER / 2); // a debounce timer for the mode button 
  digitalWrite (signalPin, LOW);  // tag the menu mode exit 
} // UI()
