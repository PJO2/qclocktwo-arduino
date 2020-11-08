// ----------------------------------
// led strip interface 
//
// call the NeoPixel library to display the time
// ----------------------------------

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Keep the constant data into the Code Segment, not in the RAM 
#include <avr/pgmspace.h>

#include "horloge.h"
#include "leds.h"

#define COTE 14
#define NUMPIXELS (COTE*COTE)


// static Adafruit_NeoPixel strip(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
static Adafruit_NeoPixel *hr_strip ; 
static Adafruit_NeoPixel *mn_strip ; 


struct S_Degrade
{
   byte r;
   byte g;
   byte b;
   byte padding;
};

const PROGMEM struct S_Degrade tColors[] =
{   
   { 128,   0, 128,  0 },     { 129,   1, 127,  0 },     { 131,   3, 125,  0 },     { 133,   5, 123,  0 },     { 135,   7, 121,  0 },     { 137,   9, 119,  0 },     { 139,  11, 117,  0 },  
   { 141,  13, 115,  0 },     { 143,  15, 113,  0 },     { 145,  17, 111,  0 },     { 147,  19, 109,  0 },     { 149,  21, 107,  0 },     { 151,  23, 105,  0 },     { 153,  25, 103,  0 },  

   { 155,  27, 101,  0 },     { 157,  29,  99,  0 },     { 159,  31,  97,  0 },     { 161,  33,  95,  0 },     { 163,  35,  93,  0 },     { 165,  37,  91,  0 },     { 167,  39,  89,  0 },  
   { 169,  41,  87,  0 },     { 171,  43,  85,  0 },     { 173,  45,  83,  0 },     { 175,  47,  81,  0 },     { 176,  48,  80,  0 },     { 178,  50,  78,  0 },     { 180,  52,  76,  0 },  

   { 182,  54,  74,  0 },     { 184,  56,  72,  0 },     { 186,  58,  70,  0 },     { 188,  60,  68,  0 },     { 190,  62,  66,  0 },     { 192,  64,  64,  0 },     { 194,  66,  62,  0 },  
   { 196,  68,  60,  0 },     { 198,  70,  58,  0 },     { 200,  72,  56,  0 },     { 202,  74,  54,  0 },     { 204,  76,  52,  0 },     { 206,  78,  50,  0 },     { 208,  80,  48,  0 },  

   { 210,  82,  46,  0 },     { 212,  84,  44,  0 },     { 214,  86,  42,  0 },     { 216,  88,  40,  0 },     { 218,  90,  38,  0 },     { 220,  92,  36,  0 },     { 222,  94,  34,  0 },  
   { 224,  96,  32,  0 },     { 225,  97,  31,  0 },     { 227,  99,  29,  0 },     { 229, 101,  27,  0 },     { 231, 103,  25,  0 },     { 233, 105,  23,  0 },     { 235, 107,  21,  0 },  

   { 237, 109,  19,  0 },     { 239, 111,  17,  0 },     { 241, 113,  15,  0 },     { 243, 115,  13,  0 },     { 245, 117,  11,  0 },     { 247, 119,   9,  0 },     { 249, 121,   7,  0 },  
   { 251, 123,   5,  0 },     { 253, 125,   3,  0 },     { 255, 127,   1,  0 },     { 255, 129,   1,  0 },     { 253, 131,   3,  0 },     { 251, 133,   5,  0 },     { 249, 135,   7,  0 },  

   { 247, 137,   9,  0 },     { 245, 139,  11,  0 },     { 243, 141,  13,  0 },     { 241, 143,  15,  0 },     { 240, 144,  16,  0 },     { 238, 146,  18,  0 },     { 236, 148,  20,  0 },  
   { 234, 150,  22,  0 },     { 232, 152,  24,  0 },     { 230, 154,  26,  0 },     { 228, 156,  28,  0 },     { 226, 158,  30,  0 },     { 224, 160,  32,  0 },     { 222, 162,  34,  0 },  

   { 220, 164,  36,  0 },     { 218, 166,  38,  0 },     { 216, 168,  40,  0 },     { 214, 170,  42,  0 },     { 212, 172,  44,  0 },     { 210, 174,  46,  0 },     { 208, 176,  48,  0 },  
   { 206, 178,  50,  0 },     { 204, 180,  52,  0 },     { 202, 182,  54,  0 },     { 200, 184,  56,  0 },     { 198, 186,  58,  0 },     { 196, 188,  60,  0 },     { 194, 190,  62,  0 },  

   { 192, 192,  64,  0 },     { 191, 193,  65,  0 },     { 189, 195,  67,  0 },     { 187, 197,  69,  0 },     { 185, 199,  71,  0 },     { 183, 201,  73,  0 },     { 181, 203,  75,  0 },  
   { 179, 205,  77,  0 },     { 177, 207,  79,  0 },     { 175, 209,  81,  0 },     { 173, 211,  83,  0 },     { 171, 213,  85,  0 },     { 169, 215,  87,  0 },     { 167, 217,  89,  0 },  

   { 165, 219,  91,  0 },     { 163, 221,  93,  0 },     { 161, 223,  95,  0 },     { 159, 225,  97,  0 },     { 157, 227,  99,  0 },     { 155, 229, 101,  0 },     { 153, 231, 103,  0 },  
   { 151, 233, 105,  0 },     { 149, 235, 107,  0 },     { 147, 237, 109,  0 },     { 145, 239, 111,  0 },     { 144, 240, 112,  0 },     { 142, 242, 114,  0 },     { 140, 244, 116,  0 },  

   { 138, 246, 118,  0 },     { 136, 248, 120,  0 },     { 134, 250, 122,  0 },     { 132, 252, 124,  0 },     { 130, 254, 126,  0 },     { 128,   0, 128,  0 },     { 126, 254, 130,  0 },  
   { 124, 252, 132,  0 },     { 122, 250, 134,  0 },     { 120, 248, 136,  0 },     { 118, 246, 138,  0 },     { 116, 244, 140,  0 },     { 114, 242, 142,  0 },     { 112, 240, 144,  0 },  

   { 110, 238, 146,  0 },     { 108, 236, 148,  0 },     { 106, 234, 150,  0 },     { 104, 232, 152,  0 },     { 102, 230, 154,  0 },     { 100, 228, 156,  0 },     {  98, 226, 158,  0 },  
   {  96, 224, 160,  0 },     {  95, 223, 161,  0 },     {  93, 221, 163,  0 },     {  91, 219, 165,  0 },     {  89, 217, 167,  0 },     {  87, 215, 169,  0 },     {  85, 213, 171,  0 },  

   {  83, 211, 173,  0 },     {  81, 209, 175,  0 },     {  79, 207, 177,  0 },     {  77, 205, 179,  0 },     {  75, 203, 181,  0 },     {  73, 201, 183,  0 },     {  71, 199, 185,  0 },  
   {  69, 197, 187,  0 },     {  67, 195, 189,  0 },     {  65, 193, 191,  0 },     {  63, 191, 193,  0 },     {  61, 189, 195,  0 },     {  59, 187, 197,  0 },     {  57, 185, 199,  0 },  

   {  55, 183, 201,  0 },     {  53, 181, 203,  0 },     {  51, 179, 205,  0 },     {  49, 177, 207,  0 },     {  48, 176, 208,  0 },     {  46, 174, 210,  0 },     {  44, 172, 212,  0 },  
   {  42, 170, 214,  0 },     {  40, 168, 216,  0 },     {  38, 166, 218,  0 },     {  36, 164, 220,  0 },     {  34, 162, 222,  0 },     {  32, 160, 224,  0 },     {  30, 158, 226,  0 },  

   {  28, 156, 228,  0 },     {  26, 154, 230,  0 },     {  24, 152, 232,  0 },     {  22, 150, 234,  0 },     {  20, 148, 236,  0 },     {  18, 146, 238,  0 },     {  16, 144, 240,  0 },  
   {  14, 142, 242,  0 },     {  12, 140, 244,  0 },     {  10, 138, 246,  0 },     {   8, 136, 248,  0 },     {   6, 134, 250,  0 },     {   4, 132, 252,  0 },     {   2, 130, 254,  0 }, 
 
};

//
// initialization 
// 
void initLeds (int WS2812_PIN_HR, int WS2812_PIN_MN)
{
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif

   hr_strip = new Adafruit_NeoPixel (MINUTES_off,             WS2812_PIN_HR, NEO_GRB + NEO_KHZ800);                 
   hr_strip->begin();            // INITIALIZE NeoPixel strip object (REQUIRED)
   hr_strip->clear();            // Turn OFF all pixels ASAP
   hr_strip->setBrightness(25);  // Set BRIGHTNESS to about 1/5 (max = 255)

   mn_strip = new Adafruit_NeoPixel (NUMPIXELS - MINUTES_off, WS2812_PIN_MN, NEO_GRB + NEO_KHZ800);              
   mn_strip->begin();            // INITIALIZE NeoPixel strip object (REQUIRED)
   mn_strip->clear();            // Turn OFF all pixels ASAP
   mn_strip->setBrightness(25);  // Set BRIGHTNESS to about 1/5 (max = 255)
}

//
// Change the brightness
//
void setBrightnessLeds (int b)
{
  hr_strip->setBrightness (b);
  mn_strip->setBrightness (b);
}

// ---------------
// display the time
// ---------------


// display time : 
// a debug version on a single strip 
// light leds at pos #{hour} and #{minute}
void _________leds_showtime (int hours, int minutes, int seconds)
{
static int count=100;
     // strip.setPixelColor ( (seconds-2) % 15,   0);
     hr_strip->clear ();
     uint32_t color = pgm_read_dword_near (tColors + count);
     if (minutes!=-1)
         hr_strip->setPixelColor (  minutes    % 15,   color ); 
     color = pgm_read_dword_near (tColors + count + NUMPIXELS/2);
     if (hours != -1)
         hr_strip->setPixelColor (  hours    % 12,   color ); 
     hr_strip->show ();
     count = (count + 1) % (NUMPIXELS/2);
}

// display a single word knowing pos and len
static void display_word (Adafruit_NeoPixel *strip, int pos, int len, int color_offset)
{
   for (int i=pos ; i<pos + len ; i++)
         strip->setPixelColor ( i ,  pgm_read_dword (tColors + i + color_offset));
} // display_word


// display all words by retrieving pos and len from data table
// if params is set to -1, it is not displayed to make a blinking effect
void showtimeLeds (int hours, int minutes, int seconds)
{
union u_Words data;

  disp_Horloge (hours, minutes, & data); // retrieve matrix
  
  hr_strip->clear();            // Turn OFF all pixels 
  display_word (hr_strip, 0, 2, 0);   // IL 
  display_word (hr_strip, 3, 3, 0);   // EST
  display_word (hr_strip, data.s.h.word1.pos, data.s.h.word1.len, 0);
  display_word (hr_strip, data.s.h.word2.pos, data.s.h.word2.len, 0);
  hr_strip->show();     

  mn_strip->clear();            // Turn OFF all pixels 
  display_word (mn_strip, data.s.m.word1.pos, data.s.m.word1.len, MINUTES_off);
  display_word (mn_strip, data.s.m.word2.pos, data.s.m.word2.len, MINUTES_off);
  display_word (mn_strip, data.s.m.word3.pos, data.s.m.word3.len, MINUTES_off);
  mn_strip->show();     
}  // leds_showtime
