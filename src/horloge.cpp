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

// Keep the constant data into the Code Segment, not in the RAM 
#include <avr/pgmspace.h>

#include "horloge.h"

/*
 * Code genere sur la base de l'horloge suivante
        "IL#ESTRUNELDIX" 
        "MINUITDEUXSEPT" 
        "QUATREMIDICING" 
        "HUIT-TROISNEUF" 
        "SIXONZE+HEURES" 
        "TRENTEQUARANTE" 
        "VINGTCINQUANTE" 
        "DIXQUATRESEPTI" 
        "UNE#TROISSEIZE" 
        "SIXEPILE!DEUXF" 
        "ONZELNEUFCHUIT" 
        "UEDOUZEACINQUE" 
        "QUATORZETREIZE" 
        "CQUINZEADEGRES" 
*/

const PROGMEM struct s_HourCode tHourWords[] = 
{
  {   {  14,   6  },   {   0,   0  },  },     // Words "MINUIT "
  {   {   7,   3  },   {  64,   5  },  },     // Words "UNE HEURE"
  {   {  20,   4  },   {  64,   6  },  },     // Words "DEUX HEURES"
  {   {  47,   5  },   {  64,   6  },  },     // Words "TROIS HEURES"
  {   {  28,   6  },   {  64,   6  },  },     // Words "QUATRE HEURES"
  {   {  89,   4  },   {  64,   6  },  },     // Words "CINQ HEURES"
  {   {  56,   3  },   {  64,   6  },  },     // Words "SIX HEURES"
  {   {  24,   4  },   {  64,   6  },  },     // Words "SEPT HEURES"
  {   {  42,   4  },   {  64,   6  },  },     // Words "HUIT HEURES"
  {   {  52,   4  },   {  64,   6  },  },     // Words "NEUF HEURES"
  {   {  11,   3  },   {  64,   6  },  },     // Words "DIX HEURES"
  {   {  59,   4  },   {  64,   6  },  },     // Words "ONZE HEURES"
  {   {  34,   4  },   {   0,   0  },  },     // Words "MIDI "
  {   {   7,   3  },   {  64,   5  },  },     // Words "UNE HEURE"
  {   {  20,   4  },   {  64,   6  },  },     // Words "DEUX HEURES"
  {   {  47,   5  },   {  64,   6  },  },     // Words "TROIS HEURES"
  {   {  28,   6  },   {  64,   6  },  },     // Words "QUATRE HEURES"
  {   {  89,   4  },   {  64,   6  },  },     // Words "CINQ HEURES"
  {   {  56,   3  },   {  64,   6  },  },     // Words "SIX HEURES"
  {   {  24,   4  },   {  64,   6  },  },     // Words "SEPT HEURES"
  {   {  42,   4  },   {  64,   6  },  },     // Words "HUIT HEURES"
  {   {  52,   4  },   {  64,   6  },  },     // Words "NEUF HEURES"
  {   {  11,   3  },   {  64,   6  },  },     // Words "DIX HEURES"
  {   {  59,   4  },   {  64,   6  },  },     // Words "ONZE HEURES"
};
const PROGMEM struct s_MinuteCode tMinuteWords[] = 
{
  {   { 130,   4  },   { 196,   0  },   { 196,   0  },  },     // Words "PILE  "
  {   { 112,   3  },   { 196,   0  },   { 196,   0  },  },     // Words "UNE  "
  {   { 135,   4  },   { 196,   0  },   { 196,   0  },  },     // Words "DEUX  "
  {   { 116,   5  },   { 196,   0  },   { 196,   0  },  },     // Words "TROIS  "
  {   { 101,   6  },   { 196,   0  },   { 196,   0  },  },     // Words "QUATRE  "
  {   { 162,   4  },   { 196,   0  },   { 196,   0  },  },     // Words "CINQ  "
  {   { 126,   3  },   { 196,   0  },   { 196,   0  },  },     // Words "SIX  "
  {   { 107,   4  },   { 196,   0  },   { 196,   0  },  },     // Words "SEPT  "
  {   { 150,   4  },   { 196,   0  },   { 196,   0  },  },     // Words "HUIT  "
  {   { 145,   4  },   { 196,   0  },   { 196,   0  },  },     // Words "NEUF  "
  {   {  98,   3  },   { 196,   0  },   { 196,   0  },  },     // Words "DIX  "
  {   { 140,   4  },   { 196,   0  },   { 196,   0  },  },     // Words "ONZE  "
  {   { 156,   5  },   { 196,   0  },   { 196,   0  },  },     // Words "DOUZE  "
  {   { 176,   6  },   { 196,   0  },   { 196,   0  },  },     // Words "TREIZE  "
  {   { 168,   8  },   { 196,   0  },   { 196,   0  },  },     // Words "QUATORZE  "
  {   { 183,   6  },   { 196,   0  },   { 196,   0  },  },     // Words "QUINZE  "
  {   { 121,   5  },   { 196,   0  },   { 196,   0  },  },     // Words "SEIZE  "
  {   {  98,   3  },   {  46,   1  },   { 107,   4  },  },     // Words "DIX - SEPT"
  {   {  98,   3  },   {  46,   1  },   { 150,   4  },  },     // Words "DIX - HUIT"
  {   {  98,   3  },   {  46,   1  },   { 145,   4  },  },     // Words "DIX - NEUF"
  {   {  84,   5  },   { 196,   0  },   { 196,   0  },  },     // Words "VINGT  "
  {   {  84,   5  },   { 175,   2  },   { 112,   2  },  },     // Words "VINGT ET UN"
  {   {  84,   5  },   { 135,   4  },   { 196,   0  },  },     // Words "VINGT DEUX "
  {   {  84,   5  },   { 116,   5  },   { 196,   0  },  },     // Words "VINGT TROIS "
  {   {  84,   5  },   { 101,   6  },   { 196,   0  },  },     // Words "VINGT QUATRE "
  {   {  84,   5  },   { 162,   4  },   { 196,   0  },  },     // Words "VINGT CINQ "
  {   {  84,   5  },   { 126,   3  },   { 196,   0  },  },     // Words "VINGT SIX "
  {   {  84,   5  },   { 107,   4  },   { 196,   0  },  },     // Words "VINGT SEPT "
  {   {  84,   5  },   { 150,   4  },   { 196,   0  },  },     // Words "VINGT HUIT "
  {   {  84,   5  },   { 145,   4  },   { 196,   0  },  },     // Words "VINGT NEUF "
  {   {  70,   6  },   { 196,   0  },   { 196,   0  },  },     // Words "TRENTE  "
  {   {  70,   6  },   { 175,   2  },   { 112,   2  },  },     // Words "TRENTE ET UN"
  {   {  70,   6  },   { 135,   4  },   { 196,   0  },  },     // Words "TRENTE DEUX "
  {   {  70,   6  },   { 116,   5  },   { 196,   0  },  },     // Words "TRENTE TROIS "
  {   {  70,   6  },   { 101,   6  },   { 196,   0  },  },     // Words "TRENTE QUATRE "
  {   {  70,   6  },   { 162,   4  },   { 196,   0  },  },     // Words "TRENTE CINQ "
  {   {  70,   6  },   { 126,   3  },   { 196,   0  },  },     // Words "TRENTE SIX "
  {   {  70,   6  },   { 107,   4  },   { 196,   0  },  },     // Words "TRENTE SEPT "
  {   {  70,   6  },   { 150,   4  },   { 196,   0  },  },     // Words "TRENTE HUIT "
  {   {  70,   6  },   { 145,   4  },   { 196,   0  },  },     // Words "TRENTE NEUF "
  {   {  76,   8  },   { 196,   0  },   { 196,   0  },  },     // Words "QUARANTE  "
  {   {  76,   8  },   { 175,   2  },   { 112,   2  },  },     // Words "QUARANTE ET UN"
  {   {  76,   8  },   { 135,   4  },   { 196,   0  },  },     // Words "QUARANTE DEUX "
  {   {  76,   8  },   { 116,   5  },   { 196,   0  },  },     // Words "QUARANTE TROIS "
  {   {  76,   8  },   { 101,   6  },   { 196,   0  },  },     // Words "QUARANTE QUATRE "
  {   {  76,   8  },   { 162,   4  },   { 196,   0  },  },     // Words "QUARANTE CINQ "
  {   {  76,   8  },   { 126,   3  },   { 196,   0  },  },     // Words "QUARANTE SIX "
  {   {  76,   8  },   { 107,   4  },   { 196,   0  },  },     // Words "QUARANTE SEPT "
  {   {  76,   8  },   { 150,   4  },   { 196,   0  },  },     // Words "QUARANTE HUIT "
  {   {  76,   8  },   { 145,   4  },   { 196,   0  },  },     // Words "QUARANTE NEUF "
  {   {  89,   9  },   { 196,   0  },   { 196,   0  },  },     // Words "CINQUANTE  "
  {   {  89,   9  },   { 175,   2  },   { 112,   2  },  },     // Words "CINQUANTE ET UN"
  {   {  89,   9  },   { 135,   4  },   { 196,   0  },  },     // Words "CINQUANTE DEUX "
  {   {  89,   9  },   { 116,   5  },   { 196,   0  },  },     // Words "CINQUANTE TROIS "
  {   {  89,   9  },   { 101,   6  },   { 196,   0  },  },     // Words "CINQUANTE QUATRE "
  {   {  89,   9  },   { 162,   4  },   { 196,   0  },  },     // Words "CINQUANTE CINQ "
  {   {  89,   9  },   { 126,   3  },   { 196,   0  },  },     // Words "CINQUANTE SIX "
  {   {  89,   9  },   { 107,   4  },   { 196,   0  },  },     // Words "CINQUANTE SEPT "
  {   {  89,   9  },   { 150,   4  },   { 196,   0  },  },     // Words "CINQUANTE HUIT "
  {   {  89,   9  },   { 145,   4  },   { 196,   0  },  },     // Words "CINQUANTE NEUF "
};


// retrieve data from code segment
static get_word_pos_and_length (struct s_Word *myword, const struct s_Word *data)
{
  myword->pos = pgm_read_byte_near ((byte *)data + offsetof (struct s_Word, pos));
  myword->len = pgm_read_byte_near ((byte *)data + offsetof (struct s_Word, len));
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
