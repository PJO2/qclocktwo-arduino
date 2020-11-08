// --------------------------------------------------------------
// horloge.c
// 
// display the time in a readable form (french) and minimum computation
// the output is processed in two steps
//    - hours are translated using tHourCodes data (the hour is the implicit index of the structure)
//    - minutes are translated using tMinutesCodes data (with minutes as index)
// The data have been produced by an external Ruby script
// -------------------------------------------------------------


#include <Arduino.h>
#include <string.h>
#include <stddef.h>

// Keep the constant data into the Code Segment, not in the RAM 
#include <avr/pgmspace.h>

#include "horloge.h"

/*
 * Code based under the following word clock
IL#ESTRUNELDIX 
MINUITDEUXSEPT 
QUATREMIDICINQ 
HUIT-TROISNEUF 
SIXONZE+HEURES 
TRENTEQUARANTE 
VINGTCINQUANTE 
DIXQUATRETCINQ 
UNE-TROISSEIZE 
SIXEPILE!DEUXF 
ONZELNEUFCHUIT 
UEDOUZEASEPTUE 
QUATORZETREIZE 
CQUINZEADEGRES 
*/

const PROGMEM struct s_HourCode tHourWords[] = 
{
  {   {  22,   6  },   {   0,   0  },  },     // Words "MINUIT "
  {   {   7,   3  },   {  64,   5  },  },     // Words "UNE HEURE"
  {   {  18,   4  },   {  64,   6  },  },     // Words "DEUX HEURES"
  {   {  46,   5  },   {  64,   6  },  },     // Words "TROIS HEURES"
  {   {  28,   6  },   {  64,   6  },  },     // Words "QUATRE HEURES"
  {   {  38,   4  },   {  64,   6  },  },     // Words "CINQ HEURES"
  {   {  56,   3  },   {  64,   6  },  },     // Words "SIX HEURES"
  {   {  14,   4  },   {  64,   6  },  },     // Words "SEPT HEURES"
  {   {  52,   4  },   {  64,   6  },  },     // Words "HUIT HEURES"
  {   {  42,   4  },   {  64,   6  },  },     // Words "NEUF HEURES"
  {   {  11,   3  },   {  64,   6  },  },     // Words "DIX HEURES"
  {   {  59,   4  },   {  64,   6  },  },     // Words "ONZE HEURES"
  {   {  34,   4  },   {   0,   0  },  },     // Words "MIDI "
  {   {   7,   3  },   {  64,   5  },  },     // Words "UNE HEURE"
  {   {  18,   4  },   {  64,   6  },  },     // Words "DEUX HEURES"
  {   {  46,   5  },   {  64,   6  },  },     // Words "TROIS HEURES"
  {   {  28,   6  },   {  64,   6  },  },     // Words "QUATRE HEURES"
  {   {  38,   4  },   {  64,   6  },  },     // Words "CINQ HEURES"
  {   {  56,   3  },   {  64,   6  },  },     // Words "SIX HEURES"
  {   {  14,   4  },   {  64,   6  },  },     // Words "SEPT HEURES"
  {   {  52,   4  },   {  64,   6  },  },     // Words "HUIT HEURES"
  {   {  42,   4  },   {  64,   6  },  },     // Words "NEUF HEURES"
  {   {  11,   3  },   {  64,   6  },  },     // Words "DIX HEURES"
  {   {  59,   4  },   {  64,   6  },  },     // Words "ONZE HEURES"
};
const PROGMEM struct s_MinuteCode tMinuteWords[] = 
{
  {   {  60,   5  },   {   0,   0  },   {   0,   0  },  },     // Words "PILE!  "
  {   {  53,   3  },   {   0,   0  },   {   0,   0  },  },     // Words "UNE  "
  {   {  65,   4  },   {   0,   0  },   {   0,   0  },  },     // Words "DEUX  "
  {   {  47,   5  },   {   0,   0  },   {   0,   0  },  },     // Words "TROIS  "
  {   {  31,   6  },   {   0,   0  },   {   0,   0  },  },     // Words "QUATRE  "
  {   {  38,   4  },   {   0,   0  },   {   0,   0  },  },     // Words "CINQ  "
  {   {  56,   3  },   {   0,   0  },   {   0,   0  },  },     // Words "SIX  "
  {   {  92,   4  },   {   0,   0  },   {   0,   0  },  },     // Words "SEPT  "
  {   {  70,   4  },   {   0,   0  },   {   0,   0  },  },     // Words "HUIT  "
  {   {  75,   4  },   {   0,   0  },   {   0,   0  },  },     // Words "NEUF  "
  {   {  28,   3  },   {   0,   0  },   {   0,   0  },  },     // Words "DIX  "
  {   {  80,   4  },   {   0,   0  },   {   0,   0  },  },     // Words "ONZE  "
  {   {  86,   5  },   {   0,   0  },   {   0,   0  },  },     // Words "DOUZE  "
  {   {  98,   6  },   {   0,   0  },   {   0,   0  },  },     // Words "TREIZE  "
  {   { 104,   8  },   {   0,   0  },   {   0,   0  },  },     // Words "QUATORZE  "
  {   { 113,   6  },   {   0,   0  },   {   0,   0  },  },     // Words "QUINZE  "
  {   {  42,   5  },   {   0,   0  },   {   0,   0  },  },     // Words "SEIZE  "
  {   {  28,   3  },   {  52,   1  },   {  92,   4  },  },     // Words "DIX - SEPT"
  {   {  28,   3  },   {  52,   1  },   {  70,   4  },  },     // Words "DIX - HUIT"
  {   {  28,   3  },   {  52,   1  },   {  75,   4  },  },     // Words "DIX - NEUF"
  {   {  23,   5  },   {   0,   0  },   {   0,   0  },  },     // Words "VINGT  "
  {   {  23,   5  },   {  36,   2  },   {  54,   2  },  },     // Words "VINGT ET UN"
  {   {  23,   5  },   {  65,   4  },   {   0,   0  },  },     // Words "VINGT DEUX "
  {   {  23,   5  },   {  47,   5  },   {   0,   0  },  },     // Words "VINGT TROIS "
  {   {  23,   5  },   {  31,   6  },   {   0,   0  },  },     // Words "VINGT QUATRE "
  {   {  23,   5  },   {  38,   4  },   {   0,   0  },  },     // Words "VINGT CINQ "
  {   {  23,   5  },   {  56,   3  },   {   0,   0  },  },     // Words "VINGT SIX "
  {   {  23,   5  },   {  92,   4  },   {   0,   0  },  },     // Words "VINGT SEPT "
  {   {  23,   5  },   {  70,   4  },   {   0,   0  },  },     // Words "VINGT HUIT "
  {   {  23,   5  },   {  75,   4  },   {   0,   0  },  },     // Words "VINGT NEUF "
  {   {   0,   6  },   {   0,   0  },   {   0,   0  },  },     // Words "TRENTE  "
  {   {   0,   6  },   {  36,   2  },   {  54,   2  },  },     // Words "TRENTE ET UN"
  {   {   0,   6  },   {  65,   4  },   {   0,   0  },  },     // Words "TRENTE DEUX "
  {   {   0,   6  },   {  47,   5  },   {   0,   0  },  },     // Words "TRENTE TROIS "
  {   {   0,   6  },   {  31,   6  },   {   0,   0  },  },     // Words "TRENTE QUATRE "
  {   {   0,   6  },   {  38,   4  },   {   0,   0  },  },     // Words "TRENTE CINQ "
  {   {   0,   6  },   {  56,   3  },   {   0,   0  },  },     // Words "TRENTE SIX "
  {   {   0,   6  },   {  92,   4  },   {   0,   0  },  },     // Words "TRENTE SEPT "
  {   {   0,   6  },   {  70,   4  },   {   0,   0  },  },     // Words "TRENTE HUIT "
  {   {   0,   6  },   {  75,   4  },   {   0,   0  },  },     // Words "TRENTE NEUF "
  {   {   6,   8  },   {   0,   0  },   {   0,   0  },  },     // Words "QUARANTE  "
  {   {   6,   8  },   {  36,   2  },   {  54,   2  },  },     // Words "QUARANTE ET UN"
  {   {   6,   8  },   {  65,   4  },   {   0,   0  },  },     // Words "QUARANTE DEUX "
  {   {   6,   8  },   {  47,   5  },   {   0,   0  },  },     // Words "QUARANTE TROIS "
  {   {   6,   8  },   {  31,   6  },   {   0,   0  },  },     // Words "QUARANTE QUATRE "
  {   {   6,   8  },   {  38,   4  },   {   0,   0  },  },     // Words "QUARANTE CINQ "
  {   {   6,   8  },   {  56,   3  },   {   0,   0  },  },     // Words "QUARANTE SIX "
  {   {   6,   8  },   {  92,   4  },   {   0,   0  },  },     // Words "QUARANTE SEPT "
  {   {   6,   8  },   {  70,   4  },   {   0,   0  },  },     // Words "QUARANTE HUIT "
  {   {   6,   8  },   {  75,   4  },   {   0,   0  },  },     // Words "QUARANTE NEUF "
  {   {  14,   9  },   {   0,   0  },   {   0,   0  },  },     // Words "CINQUANTE  "
  {   {  14,   9  },   {  36,   2  },   {  54,   2  },  },     // Words "CINQUANTE ET UN"
  {   {  14,   9  },   {  65,   4  },   {   0,   0  },  },     // Words "CINQUANTE DEUX "
  {   {  14,   9  },   {  47,   5  },   {   0,   0  },  },     // Words "CINQUANTE TROIS "
  {   {  14,   9  },   {  31,   6  },   {   0,   0  },  },     // Words "CINQUANTE QUATRE "
  {   {  14,   9  },   {  38,   4  },   {   0,   0  },  },     // Words "CINQUANTE CINQ "
  {   {  14,   9  },   {  56,   3  },   {   0,   0  },  },     // Words "CINQUANTE SIX "
  {   {  14,   9  },   {  92,   4  },   {   0,   0  },  },     // Words "CINQUANTE SEPT "
  {   {  14,   9  },   {  70,   4  },   {   0,   0  },  },     // Words "CINQUANTE HUIT "
  {   {  14,   9  },   {  75,   4  },   {   0,   0  },  },     // Words "CINQUANTE NEUF "
};

void init_Horloge ()
{
   // empty
}

// retrieve data from code segment
static void get_word_pos_and_length (struct s_Word *myword, const struct s_Word *data)
{
  myword->pos = pgm_read_byte ((byte *)data + offsetof (struct s_Word, pos));
  myword->len = pgm_read_byte ((byte *)data + offsetof (struct s_Word, len));
} // get_word_pos_and_length 


// retrieve the pos and length of the words to be displayed 
// according to the given time 
// if data are out of bounds, 0,0 is returned
union u_Words * disp_Horloge (int hours, int minutes, union u_Words *clk)
{
   memset (clk, 0, sizeof *clk);
   if (hours>=0 && hours < 24)
   {
      get_word_pos_and_length (& clk->s.h.word1, & tHourWords[hours].word1);
      get_word_pos_and_length (& clk->s.h.word2, & tHourWords[hours].word2);
   }
   if (minutes>=0 && minutes < 60)
   {
      get_word_pos_and_length (& clk->s.m.word1, & tMinuteWords[minutes].word1);
      get_word_pos_and_length (& clk->s.m.word2, & tMinuteWords[minutes].word2);
      get_word_pos_and_length (& clk->s.m.word3, & tMinuteWords[minutes].word3);
   }
return clk;
} // disp_Horloge 
