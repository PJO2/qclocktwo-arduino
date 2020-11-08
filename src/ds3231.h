// ---------------------------
// A small DS3231 manager
// Ph. Jounin  2020/11/03
// ---------------------------

// #define SizeOfTab(x) (sizeof x / sizeof x[0])

struct S_hrminsec
{
  uint8_t hours;
  uint8_t minutes;
  uint8_t seconds;
};


#define DS3231_ADDR  0x68                 // I2C address od the DS3231 RTC clock page 11

#define DS3231_24H   (0<<6)               // 24h format (see datasheet)

// DS3231 registers starting from address 0
enum { 
 DS3231_SECONDS, DS3231_MINUTES, DS3231_HOURS, DS3231_DAY, DS3231_DATE, DS3231_CEN_MONTH, DS3231_DEC_YEAR,
 DS3231_ALARM1_SECONDS, DS3231_ALARM1_MINUTES, DS3231_ALARM1_HOURS, DS3231_ALARM1_DAY_DATE,
 DS3231_ALARM2_MINUTES, DS3231_ALARM2_HOURS, DS3231_ALARM2_DAY_DATE,
 DS3231_CONTROL, DS3231_STATUS,
 DS3231_AGING_OFFSET, DS3231_TEMP_MSB,  DS3231_TEMP_LSB  
};


uint8_t DS3231_readRegister(uint8_t reg);
void DS3231_writeRegister(uint8_t reg, uint8_t data) ;
void DS3231_setTime (int hours, int minutes, int seconds) ;
struct S_hrminsec DS3231_getTime ();
void DS3231_setAlarmEverySecond();
void DS3231_setAlarmEveryMinute();
void initDS3231 ();

void DS3231_clearA1F();      // do NOT use inside interruption routine
void DS3231_clearA2F();      // do NOT use inside interruption routine

void DS3231_DisplayTimeToSerial(const struct S_hrminsec *ptm);  // for debug
