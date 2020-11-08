// ---------------------------
// led strip instruction
// Ph. Jounin  2020/11/03
// ---------------------------

// screen is splitted in two
// the first 5 rows for the hours 
// the rest for the minutes
#define MINUTES_off 5*14

void initLeds (int pinHour, int pinMinute);
void showtimeLeds (int hours, int minutes, int seconds);
void setBrightnessLeds (int);
