// ---------------
// logger.h
// A minimalist arduino console logger 
// PJO, 2020/11/08
// ---------------



#include <avr/pgmspace.h>     // do not forget that fmt is stored in code

#define LOGSIZE 64

enum { logNONE, logFATAL, logERROR, logWARN, logINFO, logDEBUG, logTRACE };

extern const unsigned int logLevel;  // defined into log.cpp

void initLog (int serialSpeed);


// The main function : 
// send a log to the serial console
//     args:
//             level : log level of the message according to syslog scale
//             fmt   : a string located in the prgm memory (ie declared with the F() macro)
//                     same format as the printf function
//             ...   : various arguments used by fmt
void Log (unsigned int level, const __FlashStringHelper *fmt, ...);
