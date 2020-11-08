// ------------------------
// a minimalist logger
// principal goal is to avoid memory consumption in release mode
// ------------------------

#include <Arduino.h>
#include <avr/pgmspace.h>
#include "logger.h"

// log level must be set in this module to make compiler strip uneeded code  
const unsigned int logLevel = logNONE;

void initLog (int serialSpeed)
{
  if (logLevel > logNONE)
     Serial.begin (serialSpeed);
}

static void doLog (const __FlashStringHelper *fmt, va_list args)
{
char sz [LOGSIZE];
    sz[sizeof sz - 1] = 0;
    vsnprintf_P (sz, sizeof sz, (const char*) fmt, args );     // read args
    Serial.println (sz);          // output to serial console    
} // doLog


void Log (unsigned int level, const __FlashStringHelper *fmt, ...)
{
va_list args;
    va_start (args, fmt );  
    // Since logLevel is declared as constant and level is declare as unsigned
    // settings logLevel to logNONE (0) should drain all function's content 
    // This can be easily check by changing logLevel and comparing code and memory size
    if (logLevel >= level)
        doLog (fmt, args);
}  // Log
